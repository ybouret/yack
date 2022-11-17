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
            }

            if(!primaryBalance(xml)) return false;
            


            //------------------------------------------------------------------
            //
            //
            // Test balance
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
                            if(c<0)
                            {
                                if(verbose) std::cerr << "[-]";
                                balanced = false;
                            }
                            else
                            {
                                if(verbose) std::cerr << "[+]";
                            }
                    }
                    if(verbose) std::cerr << std::endl;

                }

                if(balanced)
                {
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
                        lattice.pad(*xml<< '<' << eq.name << '>',eq) << " : unbalanced"
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
                        if(c<0)
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

                if(!vanish || factor<=0)
                {
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
                    Ci[j] = c + d * factor;
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
                    for(const anode *node = working.head;node;node=node->next)
                    {
                        const species &s = **node;
                        const size_t   j = *s;
                        const double   c0 = C0[j];
                        const double   c1 = Cbal[j];
                        corelib.pad(*xml << '[' << s.name << ']',s) << " = " << std::setw(15) << c0 << " -> " << std::setw(15) << c1 << std::endl;
                    }
                }
                return false;
            }

            YACK_XMLOG(xml,"-- using <" << champion->name << "> @cycle #" << cycle);
            working.transfer(Cbal,Ceq[**champion]);
            goto CYCLE;

        }

    }

}


