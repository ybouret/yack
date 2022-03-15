

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        bool plexus:: inverseOmega0() throw()
        {
            iOmega.assign(Omega0);
            for(size_t i=N;i>0;--i)
            {
                iOmega[i][i] *= xa[i];
            }
            return LU.build(iOmega);
        }
        


        bool plexus:: solve(writable<double> &C) throw()
        {
            assert(C.size()>=M);
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << "Cini=",C);
            switch(N)
            {
                case 0:
                    goto SUCCESS;

                case 1: {
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C,Ctry);
                    transfer(C,Ctry);
                } goto SUCCESS;


                default:
                    break;
            }

            {


                const double rms = computeMissing(C);
                std::cerr << "rms=" << rms << std::endl;
                computeOmega0();
                xa.ld(1);

            EVAL_XI:
                if(!inverseOmega0())
                {
                    return false;
                }

                tao::v1::set(xi,Xi);
                LU.solve(iOmega,xi);
                std::cerr << "xi=" << xi << " // factor=" << xa << std::endl;

                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const limits      &lm = eq.primary_limits(C,lib.width);
                    const size_t       ei = *eq;
                    if(entity::verbose)
                    {
                        eqs.pad(std::cerr << "//   " << eq.name, eq) << " : " << lm << std::endl;
                    }
                    switch(lm.type)
                    {
                        case limited_by_none:
                            break;

                        case limited_by_reac:
                            if(xi[ei]>lm.reac_extent())
                            {
                                YACK_CHEM_PRINTLN("//    |_reactant overload");
                                xa[ei] *= 10;
                                goto EVAL_XI;
                            }
                            break;

                        case limited_by_prod:
                            if(xi[ei]<lm.prod_extent())
                            {
                                YACK_CHEM_PRINTLN("//    |_product overload");
                                xa[ei] *= 10;
                                goto EVAL_XI;
                            }
                            break;

                        case limited_by_both: {
                            const double x = xi[ei];
                            if(x<lm.prod_extent()||x>lm.reac_extent())
                            {
                                YACK_CHEM_PRINTLN("//    |_component overload");
                                xa[ei] *= 10;
                                goto EVAL_XI;
                            }
                        } break;
                    }

                }

                if(!computeDeltaC(C))
                {
                    for(size_t i=N;i>0;--i) xa[i] *= 10;
                    goto EVAL_XI;
                }

                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    Ctry[j] = C[j] + dC[j];
                }
                
                const double new_rms = computeMissing(Ctry);
                std::cerr << "new_rms=" << new_rms << " / " << rms << std::endl;


                exit(1);
                return false;
            }

        SUCCESS:
            computeState(C);
            if(verbose)
            {
                lib(std::cerr << "Cini=",C);
                eqs(std::cerr << "Gamma=",Gamma);
            }
            YACK_CHEM_PRINTLN("// <plexus.solve/>");
            return true;
        }


    }

}
