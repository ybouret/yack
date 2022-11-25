#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"

#include "yack/type/boolean.h"
#include "yack/exception.hpp"
#include "yack/math/numeric.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {


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

        bool reactor:: isWellBalanced(const xmlog &xml) const throw()
        {
            YACK_XMLSUB(xml,"ComputeBalance");
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
            YACK_XMLOG(xml, "-- balanced = " << yack_boolean(balanced) );
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

        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");


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

            meta_repo<const equilibrium> win(N);
            writable<double> &dB       = Ctry;
            double            injected = preserved(Cbal,xml);

            
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
            win.free();
            for(const enode *node = lattice.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                dB[ei] = 0;
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
                    
                    if(verbose)
                        lattice.pad(*xml<< '<' << eq.name << '>',eq)
                        << " | #reac = " << std::setw(4) << nr
                        << " | #prod = " << std::setw(4) << np;

                    //----------------------------------------------------------
                    // act accordingly
                    //----------------------------------------------------------
                    if(nr>0)
                    {
                        if(np>0)
                        {
                            if(verbose) std::cerr << " | [blocked]" << std::endl;
                            continue; // can't use this equilibirum
                        }
                        if(verbose) std::cerr << " | [reverse]" << std::endl;
                        iota::neg(beta,NuL[ei]);
                    }
                    else
                    {
                        assert(nr<=0);
                        if(np<=0)
                        {
                            if(verbose) std::cerr << " | [regular]" << std::endl;
                            continue; // useless state
                        }
                        if(verbose) std::cerr << " | [forward]" << std::endl;
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
                YACK_XMLOG(xml, "|_" << beta);
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const species &s = ****cn;       // the species
                    const size_t   j = *s;           // its index
                    const int      d = beta[j];      // the direction
                    const double   c = Cbal[j];      // the concentration
                    const bool     increasing = d>0; // the current direction

                    if(verbose)
                    {
                        corelib.pad( *xml << "|_[" << (increasing?'+':'-') << "] [" << s.name << "]",s) << " = " << std::setw(15) << c << ' ';
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
                            if(verbose) std::cerr << "^growth^" << std::endl;
                            continue; // won't use this SPECIES
                        }
                        else
                        {
                            assert(c<0);
                            const double f = (-c)/d;
                            updateFactor(factor,vanish,f,s);
                            if(verbose) std::cerr << "increase @" << std::setw(15) << f << std::endl;
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
                            if(verbose) std::cerr << "decrease @" << std::setw(15) << f << std::endl;
                        }
                        else
                        {
                            assert(c<=0);
                            if(verbose) std::cerr << "defunct!" << std::endl;
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
                    YACK_XMLOG(xml, "|_discarding...");
                    assert(0==vanish->size);
                    continue;
                }
                
                YACK_XMLOG(xml, "|_[*] " << vanish.list << " with factor=" << factor);
                
                assert(factor>0);
                assert(vanish->size>0);
                
                
                
                //--------------------------------------------------------------
                //
                // construct trial
                //
                //--------------------------------------------------------------
                writable<double> &Ci = Ceq[ei];
                iota::load(Ci,Cbal);
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const species &s = ****cn;
                    const size_t   j = *s;
                    const int      d = beta[j];
                    double        &c = Ci[j];
                    if(d>0)
                    {
                        if(c>=0)
                        {
                            // any increase
                            c += factor * d;
                        }
                        else
                        {
                            assert(c<0);
                            // never increase more than 0
                            c = min_of( c += d*factor, 0.0);
                        }
                    }
                    else
                    {
                        assert(d<0);
                        assert(c>0);
                        c = max_of( c += factor*d, 0.0);
                    }
                }
                
                // careful
                for(const sp_node *sn=vanish->head;sn;sn=sn->next)
                {
                    Ci[ ***sn ] = 0;
                }
                
                const double B0 = eq.balance_of(Cbal,xadd);
                const double B1 = eq.balance_of(Ci,  xadd);
                std::cerr << "\tB0=" << B0 << " @" << Cbal << std::endl;
                std::cerr << "\tB1=" << B1 << " @" << Ci << std::endl;
                
                const double gain = dB[ei] = B0 - B1;
                std::cerr << "\tdB=" << gain << std::endl;
                if(gain>0)
                {
                    win.push_back(eq);
                }
            }
            
            for(const meta_node<const equilibrium> *ep = win->head; ep; ep=ep->next)
            {
                const equilibrium &eq = **ep;
                const size_t       ei = *eq;
                lattice.pad(std::cerr << eq.name,eq) << " => "
                << std::setw(15)  << dB[ei]
                << ", change=" << std::setw(15) << eq.change_of(Ceq[ei], Cbal, xadd) << std::endl;
            }
            
            

            exit(0);
            return false;

        }



#if 0
        static inline
        double getBalance(const equilibrium      &eq,
                          const readable<double> &C,
                          raddops                &xadd)
        {
            xadd.free();
            for(const cnode *node = eq.head();node;node=node->next)
            {
                const double c = C[*****node];
                if(c<0) xadd << (-c);
            }
            return xadd.get();
        }


        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");


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

            if(!primaryBalance(xml)) return false;
            


            //------------------------------------------------------------------
            //
            //
            // check total balance
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- balancing cycle #" << cycle << " --------");
            {
                bool balanced = true;
                for(const anode *node = working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const double   c = Cbal[*s];
                    if(verbose) corelib.pad(*xml << '[' << s.name << ']',s) << " = " << std::setw(15) << c << ' ';
                    switch(s.rank)
                    {
                        case 0:
                            if(verbose) std::cerr << "[?]";
                            break;

                        case 1:
                            if(verbose) std::cerr << "[*]";
                            assert(c>=0);
                            break;

                        default:
                            if(c<0) {
                                if(verbose) std::cerr << "[-]";
                                balanced = false;
                            }
                            else {
                                if(verbose) std::cerr << "[+]";
                            }
                    }
                    if(verbose) std::cerr << std::endl;

                }

                if(balanced) {
                    YACK_XMLOG(xml, "-- <balanced>");
                    working.transfer(C0,Cbal);
                    return true;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // Loop over lattice
            //
            //
            //------------------------------------------------------------------
            const equilibrium *champion = NULL;
            double             topScore = 0;

            for(const enode *node = lattice.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;

                //--------------------------------------------------------------
                //
                // load in beta the trial topology
                //
                //--------------------------------------------------------------
                {
                    // check invalid reactant
                    size_t nr = 0;
                    for(const actor *a=eq.reac->head;a;a=a->next)
                    {
                        if(Cbal[***a]<0) ++nr;
                    }

                    size_t np = 0;
                    for(const actor *a=eq.prod->head;a;a=a->next)
                    {
                        if(Cbal[***a]<0) ++np;
                    }

                    if(verbose)
                        lattice.pad(*xml<< '<' << eq.name << '>',eq)
                        << " | #reac = " << std::setw(4) << nr
                        << " | #prod = " << std::setw(4) << np;

                    if(nr>0)
                    {
                        if(np>0)
                        {
                            if(verbose) std::cerr << " | blocked" << std::endl;
                            continue;
                        }
                        if(verbose) std::cerr << " | reverse" << std::endl;
                        iota::neg(beta,NuL[ei]);
                    }
                    else
                    {
                        assert(nr<=0);
                        if(np<=0)
                        {
                            if(verbose) std::cerr << " | regular" << std::endl;
                            continue;
                        }
                        if(verbose) std::cerr << " | forward" << std::endl;
                        iota::load(beta,NuL[ei]);
                    }
                }

                //--------------------------------------------------------------
                //
                // find maximum extent with sparsity
                //
                //--------------------------------------------------------------
                assert( iota::dot<int>::of(beta,beta) > 0 );

                const species   *vanish = NULL;
                double           factor = 0;
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    const int      d = beta[j]; if(!d) continue;
                    const double   c = Cbal[j];
                    if(verbose) corelib.pad(std::cerr << "|\t[" << s.name << "]",s) << " = " << std::setw(15) << c << " with " << std::setw(4) << d << ' ';
                    if(d<0)
                    {
                        //------------------------------------------------------
                        // decreasing coefficient
                        //------------------------------------------------------
                        if(c<0)
                        {
                            //--------------------------------------------------
                            // with a negative concentration => bad
                            //--------------------------------------------------
                            if(verbose) std::cerr << "[discard]" << std::endl;
                            vanish = NULL;
                            break;
                        }
                        else
                        {
                            //--------------------------------------------------
                            // with a positive concentration => cut
                            //--------------------------------------------------
                            const double x = c/(-d);
                            if(!vanish||x<factor)
                            {
                                vanish = &s;
                                factor =  x;
                            }
                            if(verbose) std::cerr << "[decrease] @" << std::setw(15) << x << std::endl;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        // increasing coefficient
                        //------------------------------------------------------
                        assert(d>0);
                        if(c<0)
                        {
                            //--------------------------------------------------
                            // with a negative concentration => cut
                            //--------------------------------------------------
                            const double x = (-c)/d;
                            if(!vanish||x<factor)
                            {
                                vanish = &s;
                                factor =  x;
                            }
                            if(verbose) std::cerr << "[increase] @" << std::setw(15) << x << std::endl;
                        }
                        else
                        {
                            //--------------------------------------------------
                            // with a positive concentration => do nothing else
                            //--------------------------------------------------
                            if(verbose) std::cerr << "[+growth+]" << std::endl;
                        }

                    }
                }


                //--------------------------------------------------------------
                //
                //  check if useful
                //
                //--------------------------------------------------------------
                if(!vanish || factor<=0) {
                    if(verbose) std::cerr << "|\t\t<defunct>" << std::endl;
                    continue;
                }

                //--------------------------------------------------------------
                //
                // guess initial balance
                //
                //--------------------------------------------------------------
                assert(vanish!=NULL);
                assert(factor>0);

                const double B0 = getBalance(eq,Cbal,xadd);
                assert(B0>0);
                if(verbose) std::cerr << "|\tB0 = " << std::setw(15) << B0 << " @ C0=" << Cbal << std::endl;

                //--------------------------------------------------------------
                //
                // store modified concentration
                //
                //--------------------------------------------------------------
                writable<double> &Ci = Ceq[ei];
                iota::load(Ci,Cbal);
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    const int      d = beta[j]; if(!d) continue;
                    const double   c = Cbal[j];
                    double        &cc = Ci[j];
                    cc = c + d * factor;
                    if(d<0)
                    {
                        assert(c>0);
                        cc = max_of(cc,0.0);
                    }
                    else
                    {
                        assert(d>0);
                        if(c<0)
                        {
                            cc = min_of(cc,0.0);
                        }
                    }

                }
                Ci[**vanish] = 0;

                //--------------------------------------------------------------
                //
                // guess modified balance
                //
                //--------------------------------------------------------------
                const double B1 = getBalance(eq,Ci,xadd);
                if(verbose) std::cerr << "|\tB1 = " << std::setw(15) << B1 << " @ C1=" << Ci << std::endl;



                //--------------------------------------------------------------
                //
                // estimate score
                //
                //--------------------------------------------------------------
                if(B1>=B0) {
                    if(verbose) std::cerr << "|\t\t<no gain>" << std::endl;
                    continue;
                }
                const double gain  = B0-B1;
                const double cost  = sqrt(working.norm2(Cbal,Ci,xadd))/working.size;
                const double score = gain - cost;
                if(verbose) {
                    std::cerr << "|\t\t<gain  = "<< std::setw(15) << gain << ", cost = " << std::setw(15) << cost << ">" << std::endl;
                    std::cerr << "|\t\t<score = "<< std::setw(15) << score << ">" << std::endl;
                }


                //--------------------------------------------------------------
                //
                // update status
                //
                //--------------------------------------------------------------
                if(!champion || score>topScore)
                {
                    champion = &eq;
                    topScore = score;
                }
                
            }

            //------------------------------------------------------------------
            //
            //
            // check is a champion was selected
            //
            //
            //------------------------------------------------------------------
            if(!champion)
            {
                YACK_XMLOG(xml, "-- <stalled> @cycle #"<< cycle);
                if(verbose)
                {
                    dC.ld(0);
                    beta.ld(0);
                    for(const anode *node = working.head;node;node=node->next)
                    {
                        const species &s = **node;
                        const size_t   j = *s;
                        const double   c0 = C0[j];
                        const double   c1 = Cbal[j];
                        corelib.pad(*xml << '[' << s.name << ']',s) << " = " << std::setw(15) << c0 << " -> " << std::setw(15) << c1 << std::endl;
                        if(c1<0)
                        {
                            dC[j]   = -c1;
                            beta[j] =   1;
                        }
                    }
                    std::cerr << "Nu   = " << Nu << std::endl;
                    std::cerr << "dC   = " << dC << std::endl;
                    std::cerr << "beta = " << beta << std::endl;
                    std::cerr << "NuL  = " << NuL  << std::endl;
                }
                return false;
            }

            YACK_XMLOG(xml,"-- using <" << champion->name << "> @cycle #" << cycle);
            working.transfer(Cbal,Ceq[**champion]);
            goto CYCLE;

        }
#endif

    }

}


