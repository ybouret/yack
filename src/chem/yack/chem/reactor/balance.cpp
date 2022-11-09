#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>
#include <cfloat>
#include "yack/type/boolean.h"
#include "yack/exception.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: Balance(const readable<double> &C)
        {
            xadd.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double c = C[j];
                if(c<0) xadd <<  (-c);
            }
            return xadd.get();
        }

        double reactor:: Balance(const double u)
        {
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = Cbal[j] + u * dC[j];
            }
            return Balance(Ctry);
        }


        bool reactor:: primaryBalance(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"PrimaryBalancing");
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
            YACK_XMLOG(xml, "==> balancing " << (primaryBalanced?yack_success:yack_failure) << " <==");
            return primaryBalanced;
        }

        bool reactor:: isWellBalanced()
        {
            xadd.free();
            bool well = true;
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Cbal[j];
                switch(s.rank)
                {
                    case 0:               break;
                    case 1: assert(c>=0); break;
                    default:
                        if(c<0)
                        {
                            xadd << -c;
                            beta[j] = 1;
                            well    = false;
                        }
                        else
                        {
                            beta[j] = 0;
                        }
                }
            }

            return well;
        }


        struct callB
        {
            reactor &self;

            double operator()(double u)
            {
                return self.Balance(u);
            }
        };
        



        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);
            
            if(N<=0)
            {
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
                beta[j] = 0;
            }
            
            //------------------------------------------------------------------
            //
            //
            // ensure first primary balance
            //
            //
            //------------------------------------------------------------------
            if(!primaryBalance(xml)) return false;
            if(verbose) corelib(*xml << "-- Cbal=","", Cbal);
            
            
            const bool well = isWellBalanced();

            if(well)
            {
                YACK_XMLOG(xml,"-- well balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                const double B0 = xadd.get();
                std::cerr << singles << std::endl;
                std::cerr << "B0="   << B0   << std::endl;
                std::cerr << "beta=" << beta << std::endl;
                //std::cerr << "Nu  =" << Nu   << std::endl;
                int betaMax = 0;
                imatrix A(M,N);
                for(size_t i=N;i>0;--i)
                {
                    const int NuBeta = xadd.dot(Nu[i],beta);
                    betaMax = max_of(betaMax,absolute(NuBeta));
                    std::cerr << "NuBeta[" << i << "]=" << NuBeta << std::endl;
                    for(size_t j=M;j>0;--j)
                    {
                        A[j][i] = Nu[i][j] * NuBeta;
                    }
                }
                //std::cerr << "betaMax=" << betaMax << std::endl;
                //std::cerr << "A=" << A << std::endl;
                callB B = { *this };
                assert( fabs(B0 - B(0)) <= 0);

                std::cerr << "A=" << A << std::endl;

                blocked.ld(false);
                vector<size_t> cond;
                vector<bool>   vbad(N,false);
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species       &s = **node;
                    const size_t         j = *s;
                    const readable<int> &Aj = A[j];
                    corelib.pad(std::cerr << s.name,s) << " : ";
                    const double rhs = -Cbal[j];
                    std::cerr << Aj << " >= " << std::setw(15) << rhs;

                    size_t nact  = 0;
                    int    clast = 0;
                    size_t ilast = 0;
                    for(size_t i=N;i>0;--i)
                    {
                        const int a = Aj[i];
                        if(!a) continue;
                        ++nact;
                        clast = a;
                        ilast = i;
                    }

                    switch(nact)
                    {
                        case 0:
                            // not involved
                            std::cerr << " | drop";
                            break;

                        case 1:
                            //std::cerr << "unique #" << ilast << std::endl;
                            if(clast<0)
                            {
                                if(rhs>=0)
                                {
                                    vbad[ilast] = true;
                                    std::cerr << " | blocking";
                                }
                                else
                                {
                                    std::cerr << " | control";
                                    cond << j;
                                }

                            }
                            else
                            {
                                assert(clast>0);
                                if(rhs>0)
                                {
                                    // TODO: NO!!!
                                    throw exception("unsolved primary!!");
                                }
                                else
                                {
                                    // true condition
                                    std::cerr << " | true";
                                }
                            }
                            break;

                        default:
                            std::cerr << " | generic";
                            cond << j;
                            break;
                    }

                    std::cerr << std::endl;



                }
                std::cerr << "cond=" << cond << " #" << cond.size() << "/" << M << std::endl;
                std::cerr << "vbad=" << vbad << std::endl;

                exit(0);
            }
            
            return false;
            
        }

    }

}

