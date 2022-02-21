
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        
        void plexus:: solve(writable<double> &C)
        {

            switch(N)
            {
                case 0: return;
                case 1: {
                    const equilibrium &eq = ***(eqs.head());
                    eq.solve(K[1],C,Ctmp);
                    computeGammaAndPsi(C);
                } return;

                default:
                    break;
            }
            assert(N>=2);

            //------------------------------------------------------------------
            //
            // fetch initial phase space
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            YACK_CHEM_PRINTLN("// <solving>");
            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }
            size_t iter = 0;
            double                g0 = computeVariance(Corg);

        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//" << std::endl << "//   [iter=" << iter << "]");
            YACK_CHEM_PRINTLN("//   g0=" << g0);
            if(g0<=0)
            {
                YACK_CHEM_PRINTLN("// < numerical success level-0 >");
                return;
            }


            //------------------------------------------------------------------
            //
            // reduction
            //
            //------------------------------------------------------------------
            shrink(g0);
            if(g0<=0)
            {
                YACK_CHEM_PRINTLN("// < numerical success level-1 >");
                tao::v1::set(C,Corg);
                return;
            }

            //------------------------------------------------------------------
            //
            // full differential state
            //
            //------------------------------------------------------------------
            computeGammaAndPsi(Corg);

            //------------------------------------------------------------------
            //
            // regularize differential state
            //
            //------------------------------------------------------------------
            if( regularize() && verbose )
            {
                lib(std::cerr << "C0=", Corg);
                g0 = gammaVariance();
                if(g0<=0)
                {
                    YACK_CHEM_PRINTLN("// < numerical success level-2 >");
                    tao::v1::set(C,Corg);
                    return;
                }
            }


            YACK_CHEM_PRINTLN("//   Gamma = " << Gamma);
            YACK_CHEM_PRINTLN("//   Psi   = " << Psi);

            //------------------------------------------------------------------
            //
            // compute clamped Xi from Psi and blocked
            //
            //------------------------------------------------------------------
            computeExtent();

            //------------------------------------------------------------------
            //
            // compute deltaC
            //
            //------------------------------------------------------------------
            computeDeltaC(xi);

            //------------------------------------------------------------------
            //
            // move
            //
            //------------------------------------------------------------------
            const double g1 = move(g0);

            //------------------------------------------------------------------
            //
            // move
            //
            //------------------------------------------------------------------
            bool                converged = true;
            static const double mtol      = minimize::get_mtol<double>();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   del = fabs(Corg[j]-Ctry[j]);
                const double   err = fabs(Ctry[j]) * mtol;
                const bool     bad = del>err;
                if(bad)
                {
                    converged = false;
                }
                Corg[j] = Ctry[j];
            }

            computeGammaAndPsi(Corg);

            if(converged)
            {
                YACK_CHEM_PRINTLN("// <composition convergence@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }


            if( g1 <= 0)
            {
                YACK_CHEM_PRINTLN("// <numerical success@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }

            if( g1 >= g0)
            {
                YACK_CHEM_PRINTLN("// <variance convergence@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }

            g0 = g1;

            goto ITER;


            exit(1);
        }

    }

}


