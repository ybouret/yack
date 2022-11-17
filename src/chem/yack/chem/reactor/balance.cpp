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



        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);


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
            }

            if(!primaryBalance(xml)) return false;
            


            //------------------------------------------------------------------
            //
            //
            // Test balance
            //
            //
            //------------------------------------------------------------------
            {
                if(verbose) corelib(*xml << "C=","",Cbal);
                bool balanced = true;
                for(const anode *node = working.head;node;node=node->next)
                {
                    const species &s = **node;
                    if( Cbal[*s] < 0)
                    {
                        balanced = false;
                        break;
                    }
                }

                if(balanced)
                {
                    YACK_XMLOG(xml, "-- balanced");
                    return true;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // Need to solve: register active reactions from lattice
            //
            //
            //------------------------------------------------------------------
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
                        lattice.pad(*xml<<eq.name,eq) << " : unbalanced"
                        <<   " #reac = " << std::setw(4) << nr
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
                std::cerr << "\tusing " << beta << std::endl; assert( iota::dot<int>::of(beta,beta) > 0 );

                const species   *vanish = NULL;
                double           factor = 0;
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    const int      d = beta[j]; if(!d) continue;
                    const double   c = Cbal[j];
                    if(verbose) corelib.pad(std::cerr << "\t[" << s.name << "]",s) << " = " << std::setw(15) << c << " with " << std::setw(4) << d << ' ';
                    if(d<0)
                    {
                        if(c<0)
                        {
                            if(verbose) std::cerr << "[discard]" << std::endl;
                            vanish = NULL;
                            break;
                        }
                        else
                        {
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
                        assert(d>0);
                        if(c<=0)
                        {
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
                            if(verbose) std::cerr << "[+growth+]" << std::endl;
                        }

                    }
                }

                if(!vanish)
                {
                    continue;
                }







            }




            exit(0);
            return false;
        }





#if 0
        double reactor:: gain(const readable<int> &lam,
                              double              &cf,
                              const species *     &sz)
        {
            assert(NULL==sz);
            assert(fabs(cf)<=0);
            
            vector<tumbler> tumblers(M,as_capacity);
            bool            discard = false;
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const int      d = lam[j]; if(!d) continue;
                const double   c = Cbal[j];
                if(verbose) corelib.pad(std::cerr << "\t[" << s.name << "]",s) << " = " << std::setw(15) << c << " with " << std::setw(4) << d << ' ';

                if(0<d)
                {
                    if(c<0)
                    {
                        // increase c<0
                        const tumbler _( (-c)/d, s);
                        tumblers << _;
                        if(verbose) std::cerr << "[increase] " << _.f;

                    }
                    else
                    {
                        // increase c>=0
                        if(verbose) std::cerr << "[produced]";
                    }
                }
                else
                {
                    assert(d<0);
                    if(c<=0)
                    {
                        // decrease c<=0 : bad!
                        if(verbose)
                        {
                            discard = true; // postpone return
                            std::cerr << "[discard!]";
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        // decrease c>0
                        const tumbler _( c/(-d), s);
                        tumblers << _;
                        if(verbose) std::cerr << "[decrease] " << _.f;
                    }
                }


                if(verbose) std::cerr << std::endl;
            }

            if(discard) { assert(verbose); return -1; }
            const size_t na = tumblers.size();
            if(na<=0)
            {

                return 0;
            }
            else
            {
                hsort(tumblers,tumbler::compare);
                const tumbler &a = tumblers.front();
                std::cerr << a << std::endl;



                xadd.free();
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const int      d = lam[j];
                    const double   c = Cbal[j];
                    Ctry[j] = c + d * a.f;
                    if(d && c<0) xadd << -c;
                }
                Ctry[ *a.s ] = 0;
                const double B0 = xadd.get();

                xadd.free();
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const int      d = lam[j];
                    const double   c = Ctry[j];
                    if(d&&c<0) xadd << -c;
                }
                const double B1 = xadd.get();
                std::cerr << "\tB0   = " << B0   << std::endl;
                std::cerr << "\tCbal = " << Cbal << std::endl;
                std::cerr << "\tCtry = " << Ctry << std::endl;
                std::cerr << "\tB1   = " << B1   << std::endl;
                const double res = B0-B1;
                if(res>0)
                {
                    cf =  a.f;
                    sz = &a.s;
                    return res;
                }
                else
                {
                    return -1;
                }
            }

        }



        class victory
        {
        public:

            const equilibrium & elite;
            const double        score;
            const double        costs;
            const double        value;

            victory(const equilibrium &eq,
                    const double       sc,
                    const double       xp) throw() :
            elite(eq),
            score(sc),
            costs(xp),
            value(score-costs)
            {
            }

            ~victory() throw()
            {
            }

            victory(const victory &_) throw() :
            elite(_.elite),
            score(_.score),
            costs(_.costs),
            value(_.value)
            {
            }

            const equilibrium & operator*() const throw() { return elite; }

            static int compare(const victory &lhs, const victory &rhs) throw()
            {
                return comparison::decreasing(lhs.value, rhs.value);
            }


        private:
            YACK_DISABLE_ASSIGN(victory);
        };


        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);


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
                beta[j] = 0;
                dC[j]   = 0;
            }

            if(!primaryBalance(xml)) return false;

            vector<int>           xd(L,0);
            imatrix               Lambda(L,M);
            vector<equilibrium *> alive(L,as_capacity);
            vector<victory>       winner(L,as_capacity);
            unsigned  cycle = 0;

        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "------- #cycle " << cycle << " --------");

            //------------------------------------------------------------------
            //
            //
            // initialize balance and directions
            //
            //
            //------------------------------------------------------------------
            {
                xadd.free();
                bool well = true;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   c = Cbal[j];
                    if(verbose) corelib.pad(std::cerr << "| [" << s.name << "]",s) << " = " << std::setw(15) << c << ' ';
                    switch(s.rank)
                    {
                        case 0: break;
                        case 1: assert(c>=0);
                            if(c<=0)
                            {
                                if(verbose) std::cerr << "(*)";
                                beta[j] = 0;
                            }
                            else
                            {
                                if(verbose) std::cerr << "(+)";
                                beta[j] = 0;
                            }
                            break;

                        default:
                            if(c<0)
                            {
                                if(verbose) std::cerr << "(-)";
                                beta[j] = 1;
                                well    = false;
                                xadd << -c;
                            }
                            else
                            {
                                if(verbose) std::cerr << "(+)";
                                beta[j] = 0;
                            }
                    }
                    if(verbose) std::cerr << std::endl;

                }

                if(well)
                {
                    YACK_XMLOG(xml,"-- balanced!");
                    working.transfer(C0,Cbal);
                    return true;
                }
            }



            const double B0 = xadd.get();
            YACK_XMLOG(xml,"-- B0     = " << B0);
            YACK_XMLOG(xml,"-- beta   = " << beta);

            // computing xd = NuL * beta
            {
                bool found  = false;
                for(const enode *node=lattice.head();node;node=node->next)
                {
                    const equilibrium &  eq = ***node;
                    const size_t         ei = *eq;
                    const readable<int> &nu = NuL[ei];
                    int                 &xx = xd[ei];
                    xx   = iota::dot<int>::of(nu,beta);
                    if(verbose) lattice.pad(std::cerr << "| <" << eq.name << ">",eq) << " = " << std::setw(4) << xx << std::endl;

                    switch( __sign::of(xx) )
                    {
                        case __zero__:
                            xx = 0;
                            break;

                        case negative:
                            found = true;
                            xx    = -1;
                            break;

                        case positive:
                            found = true;
                            xx    = 1;
                            break;
                    }



                }
                YACK_XMLOG(xml,"-- found  = " << yack_boolean(found));
                if(!found)
                {
                    return false;
                }
            }

            // computing Lambda matrix = NuL' * diagm(xd)
            std::cerr << "xd =" << xd  << std::endl;

            alive.free();
            for(const enode *node=lattice.head();node;node=node->next)
            {
                const equilibrium   &eq    = ***node;
                const size_t         ei    = *eq;
                const int            d = xd[ei];
                writable<int>       &l = Lambda[ei];
                const readable<int> &n = NuL[ei];
                int lmax = 0;
                for(size_t j=M;j>0;--j)
                {
                    lmax = max_of(lmax, absolute( l[j] = n[j] * d ) );
                }
                if(lmax) alive << &coerce(eq);
            }


            std::cerr << "Lambda = " << Lambda << std::endl;
            std::cerr << "C      = " << Cbal   << std::endl;
            std::cerr << "@alive = " << alive.size() << "/" << L << std::endl;


            winner.free();
            const size_t na = alive.size();
            for(size_t ia=1;ia<=na;++ia)
            {
                const equilibrium   &eq    = *alive[ia];
                const size_t         ei    = *eq;
                const readable<int> &lam   = Lambda[ei];
                if(verbose) lattice.pad(std::cerr << '<' << eq.name <<'>',eq) << " : " << lam << std::endl;

                const species *sz = NULL;
                double         cf = 0;
                const double   g  = gain(lam,cf,sz);
                std::cerr << "\t\tgain=" << g << std::endl;
                if(g>0)
                {
                    iota::load(Ceq[ei],Ctry);
                    const double  score = g;
                    //const double  costs =xadd.map_to(Ctry,Cbal,abs_of<double>);
                    const double costs = sqrt(xadd.squares(Ctry,Cbal)/working.size);
                    const victory player(eq,score,costs);
                    winner << player;
                }
            }

            const size_t nw = winner.size();
            if(nw<=0)
            {
                YACK_XMLOG(xml, "-- unable to gain balance");
                corelib(std::cerr,"",Cbal);
                return false;
            }

            hsort(winner,victory::compare);
            for(size_t i=1;i<=nw;++i)
            {
                const victory &V = winner[i];
                lattice.pad(std::cerr << V.elite.name, V.elite)
                << " : score = " << std::setw(15) << V.score
                << " : costs = " << std::setw(15) << V.costs
                << " : value = " << std::setw(15) << V.value
                << std::endl;
            }

            const equilibrium &champion = *winner.front();
            std::cerr << "using <" << champion.name << ">" << std::endl;
            working.transfer(Cbal,Ceq[*champion]);
            goto CYCLE;

        }
#endif



    }

}


