#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"

#include "yack/type/boolean.h"
#include "yack/exception.hpp"
#include "yack/math/numeric.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/associative/addrbook.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

#if 0
        bool reactor:: primaryBalance(const xmlog &xml)
        {
            static const char fn[] = "primaryBalance";
            YACK_XMLSUB(xml,fn);
            bool primaryBalanced = true;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                YACK_XMLSUB(xml,eq.name);
                if(!eq.try_primary_balance(Cbal,xml))
                {
                    primaryBalanced = false;
                }
            }
            YACK_XMLOG(xml, "==> " << fn << ' ' << (primaryBalanced?yack_success:yack_failure) << " <==");
            return primaryBalanced;
        }
#endif

        bool reactor:: isWellBalanced(const xmlog &xml) const throw()
        {
            YACK_XMLSUB(xml,"balancing.check");
            bool balanced = true;
            for(const anode *node = working.head;node;node=node->next)
            {
                const species &s = **node;
                const double   c = Cbal[*s];
                if(verbose) corelib.pad(*xml << '[' << s.name << ']',s) << " = " << std::setw(15) << c << ' ';
                if(c<0)
                {
                    balanced = false;
                    if(verbose) std::cerr << "[-]";
                }
                else
                {
                    if(verbose) std::cerr << "[+]";
                }
                if(verbose) std::cerr << std::endl;
            }
            YACK_XMLOG(xml, "--> balanced = " << yack_boolean(balanced)  << " <--");
            return balanced;
        }

        static inline
        void updateFactor(double      &factor,
                          sp_repo     &vanish,
                          const double   f,
                          const species &s)
        {
            assert(f>0);
            if(factor<=0)
            {
                // initialize
                factor = f;
                vanish.free();
                vanish.push_back(s);
            }
            else
            {
                assert(factor>0);

                switch(__sign::of(f,factor) )
                {
                    case negative: // winner
                        assert(f<factor);
                        factor = f;
                        vanish.free();
                        vanish.push_back(s);
                        break;

                    case __zero__: // ex-aequo
                        vanish.push_back(s);
                        break;

                    case positive: // looser, do not update
                        assert(vanish.list.size>0);
                        break;
                }
            }
        }
        

        static inline
        bool oneIn(const group &g, const addrbook &edb) throw()
        {
            for(const gnode *gn=g.head;gn;gn=gn->next)
            {
                const equilibrium &eq = **gn;
                if(edb.search(&eq)) return true;
            }
            return false;
        }


        static inline
        double gainOf(const group            &g,
                      const readable<double> &Gain,
                      raddops                &xadd)
        {
            xadd.free();
            for(const gnode *gn=g.head;gn;gn=gn->next)
            {
                const equilibrium &eq = **gn;
                const double       eg = Gain[ *eq ]; assert(eg>=0);
                xadd << eg;
            }
            return xadd.get();
        }


        bool reactor:: balance(writable<double> &C0)
        {
            static const char   fn[] = "balancing";
            const xmlog         xml(clid,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"balancing");


            //------------------------------------------------------------------
            //
            //
            // Check Status
            //
            //
            //------------------------------------------------------------------
            if(N<=0) {
                YACK_XMLOG(xml,"-- no equilibrium");
                return true;
            }

            //------------------------------------------------------------------
            //
            //
            // initialize phase space
            //
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Cbal[j] = Ctry[j] = C0[j];
                dC[j]   = 0;
            }

            double   injected = preserved(Cbal,xml);
            unsigned cycle    = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- " << fn << " cycle #" << std::setw(4) << cycle << " | injected = " << std::setw(15) << injected);
            if(isWellBalanced(xml))
            {
                working.transfer(C0,Cbal);
                return true;
            }


            //------------------------------------------------------------------
            //
            //
            // looping over all equilibria
            //
            //
            //------------------------------------------------------------------
            addrbook edb;
            edb.free();
            Gain.ld(0);
            Cost.ld(0);
            for(const enode *node = lattice.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;

                //--------------------------------------------------------------
                //
                // load in beta the trial topology with the proper direction
                //
                //--------------------------------------------------------------
                {
                    //----------------------------------------------------------
                    // check invalid reactants
                    //----------------------------------------------------------
                    size_t nr = 0;
                    for(const actor *a=eq.reac->head;a;a=a->next)
                    {
                        if(Cbal[***a]<0) ++nr;
                    }

                    //----------------------------------------------------------
                    // check invalid products
                    //----------------------------------------------------------
                    size_t np = 0;
                    for(const actor *a=eq.prod->head;a;a=a->next)
                    {
                        if(Cbal[***a]<0) ++np;
                    }
                    
                    if(verbose) {
                        lattice.pad(*xml<< '<' << eq.name << '>',eq)
                        << " | #reac = " << std::setw(4) << nr
                        << " | #prod = " << std::setw(4) << np;
                    }

                    //----------------------------------------------------------
                    // act accordingly
                    //----------------------------------------------------------
                    if(nr>0)
                    {
                        if(np>0)
                        {
                            if(verbose) std::cerr << " | [blocked] | " << eq.content() << std::endl;
                            continue; // can't use this equilibirum
                        }
                        if(verbose) std::cerr << " | [reverse] | " << eq.content() << std::endl;
                        iota::neg(beta,NuL[ei]);
                    }
                    else
                    {
                        assert(nr<=0);
                        if(np<=0)
                        {
                            if(verbose) std::cerr << " | [regular] | " << eq.content() << std::endl;
                            continue; // useless state
                        }
                        if(verbose) std::cerr << " | [forward] | " << eq.content() << std::endl;
                        iota::load(beta,NuL[ei]);
                    }
                }


                assert( iota::dot<int>::of(beta,beta) > 0 );

                //--------------------------------------------------------------
                //
                // find how much we can improve
                //
                //--------------------------------------------------------------
                vanish.free();
                double factor = -1;
                YACK_XMLOG(xml, "  |_" << beta);
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const species &s = ****cn;       // the species
                    const size_t   j = *s;           // its index
                    const int      d = beta[j];      // the direction
                    const double   c = Cbal[j];      // the concentration
                    const bool     increasing = d>0; // the current direction

                    if(verbose)
                    {
                        corelib.pad( *xml << "  |_[" << (increasing?'+':'-') << "] [" << s.name << "]",s) << " = " << std::setw(15) << c << " | ";
                    }

                    if(increasing)
                    {
                        //------------------------------------------------------
                        //
                        // increasing species concentration
                        //
                        //------------------------------------------------------
                        if(c>=0)
                        {
                            if(verbose) std::cerr << "^growth^ [*]" << std::endl;
                            continue; // won't use this SPECIES
                        }
                        else
                        {
                            assert(c<0);
                            const double f = (-c)/d;
                            updateFactor(factor,vanish,f,s);
                            if(verbose) std::cerr << "increase [+]" << std::setw(15) << f << std::endl;
                        }
                    }
                    else
                    {
                        assert(d<0);
                        //------------------------------------------------------
                        //
                        // decreasing species concentration
                        //
                        //------------------------------------------------------
                        if(c>0)
                        {
                            const double f = c/(-d);
                            updateFactor(factor,vanish,f,s);
                            if(verbose) std::cerr << "decrease [-]" << std::setw(15) << f << std::endl;

                        }
                        else
                        {
                            assert(c<=0);
                            if(verbose) std::cerr << "defunct  [!]" << std::endl;
                            factor = -1;
                            vanish.free();
                            break; // won't use this EQUILIBRIUM
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // evaluate gain
                //
                //--------------------------------------------------------------
                if(factor<=0)
                {
                    YACK_XMLOG(xml, "  |_discarding...");
                    assert(0==vanish->size);
                    continue;
                }
                
                YACK_XMLOG(xml, "  |_[*] " << vanish.list << " with factor=" << factor);
                
                assert(factor>0);
                assert(vanish->size>0);
                
                
                
                //--------------------------------------------------------------
                //
                // construct trial
                //
                //--------------------------------------------------------------
                writable<double> &Ci = Ceq[ei];
                iota::load(Ci,Cbal);

                xadd.free();
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const species &s  = ****cn;
                    const size_t   j  = *s;
                    const int      d  = beta[j];
                    double        &c  = Ci[j];
                    const double   dc = d * factor;
                    if(d>0)
                    {
                        if(c>=0)
                        {
                            // any increase
                            c += dc;
                        }
                        else
                        {
                            assert(c<0);
                            // never increase more than 0
                            c = min_of( c += dc, 0.0);
                        }
                    }
                    else
                    {
                        assert(d<0);
                        assert(c>0);
                        c = max_of( c += dc, 0.0);
                    }
                    xadd << fabs(dc);
                }
                
                // careful with vanishing components
                for(const sp_node *sn=vanish->head;sn;sn=sn->next)
                {
                    Ci[ ***sn ] = 0;
                }

                const double cost = xadd.get();
                const double B0   = eq.balance_of(Cbal,xadd);
                const double B1   = eq.balance_of(Ci,  xadd);
                std::cerr << "\tB0=" << B0 << " @" << Cbal << std::endl;
                std::cerr << "\tB1=" << B1 << " @" << Ci << std::endl;
                const double gain =   B0 - B1;
                std::cerr << "\tgain=" << gain << " / cost=" << cost <<  std::endl;
                if(gain>0)
                {
                    if(!edb.insert(&eq)) throw exception("%s: multiple <%s>", fn, eq.name());
                    Gain[ei] = gain;
                    Cost[ei] = cost;
                }
            }

            //------------------------------------------------------------------
            //
            // analyze positive gain(s)
            //
            //------------------------------------------------------------------
            if((*edb).size<=0)
            {
                std::cerr << "STALLED" << std::endl;
                corelib(std::cerr,"",Cbal);
                preserved(Cbal,xml);
                std::cerr << std::endl;
                exit(0);
            }


            if(verbose)
            {
                YACK_XMLOG(xml, "-- summary of positive gains:");
                for(addrbook::const_iterator it=edb.begin();it!=edb.end();++it)
                {
                    const equilibrium &eq = *static_cast<const equilibrium *>(*it);
                    const size_t       ei = *eq;
                    lattice.pad(*xml << "|_" << eq.name,eq)
                    << " | gain " << std::setw(15) << Gain[ei]
                    << " | cost " << std::setw(15) << Cost[ei]
                    << std::endl;
                }
            }

            //------------------------------------------------------------------
            // find first
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- finding best group containing a positive gain:");
            const group     *best = NULL;
            double           gmax = 0;
            for(const group *g    = solving.head; g; g=g->next)
            {
                const bool ok = oneIn(*g,edb);
                if(!ok)
                {
                    //YACK_XMLOG(xml, "|_discarding " <<*g);
                    continue;
                }
                else
                {
                    best = g;
                    gmax = gainOf(*g,Gain,xadd);
                    YACK_XMLOG(xml, "|_initialize " <<*g << " @" << std::setw(15) << gmax << " <--");
                    break;
                }
            }
            assert(best); // mandatory

            //------------------------------------------------------------------
            // find better
            //------------------------------------------------------------------
            for(const group *g=best->next;g;g=g->next)
            {
                const bool ok = oneIn(*g,edb);
                if(!ok)
                {
                    //YACK_XMLOG(xml, "|_discarding " <<*g);
                    continue;
                }
                else
                {
                    const double gtmp = gainOf(*g,Gain,xadd);
                    if(gtmp>gmax)
                    {
                        gmax = gtmp;
                        best = g;
                        YACK_XMLOG(xml, "|_upgrade to " <<*g << " @" << std::setw(15) << gmax);
                    }
                    else
                    {
                        //YACK_XMLOG(xml, "|_not better " <<*g << " @" << std::setw(15) << gtmp);
                    }
                }
            }

            //------------------------------------------------------------------
            // selected
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- upgrading " << *best << " @" << std::setw(15) << gmax << " <--");


            if(verbose) iota::load(Ctry,Cbal);
            for(const gnode *gn=best->head;gn;gn=gn->next)
            {
                const equilibrium      &eq = **gn; if(!edb.search(&eq)) continue;
                const size_t            ei = *eq;  assert( Gain[ei] > 0);
                const readable<double> &Ci = Ceq[ei];
                eq.transfer(Cbal,Ci);
            }

            if(verbose)
            {
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j =  *s;
                    const double   c_old = Ctry[j];
                    const double   c_new = Cbal[j];
                    if( fabs(c_old-c_new) <= 0 ) continue;
                    corelib.pad(*xml << " (*) [" << s.name << "]", s)
                    << " = "   << std::setw(15) << c_old
                    << " --> " << std::setw(15) << c_new
                    << std::endl;
                }
            }

            goto CYCLE;

        }


    }

}


