#include "yack/chem/plexus.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/exception.hpp"
#include "yack/ios/fmt/align.hpp"
#include <iomanip>

namespace yack
{

    using namespace math;

    namespace chemical
    {
        void plexus:: computeExtent()
        {
            YACK_CHEM_PRINTLN("//   <computeExtent>");
            //------------------------------------------------------------------
            //
            // Phi*NuT
            //
            //------------------------------------------------------------------
            tao::v3::mmul_trn(W,Psi,Nu);
            for(size_t i=blocked.size();i>0;--i)
            {
                const size_t k = blocked[i];
                W[k][k]  = 1.0;
                Gamma[k] = 0.0;
            }
            YACK_CHEM_PRINTLN("  PsiNuT = " << W);

            //------------------------------------------------------------------
            //
            // inverse
            //
            //------------------------------------------------------------------
            if(!LU.build(W))
            {
                throw exception("%s: singular concentrations", clid);
            }

            //------------------------------------------------------------------
            //
            // xi = -inv(Phi*NuT)*Gamma
            //
            //------------------------------------------------------------------
            tao::v1::neg(xi,Gamma);
            LU.solve(W,xi);
            YACK_CHEM_PRINTLN("  xi0    = " << xi);

            //------------------------------------------------------------------
            //
            // validate Xi with primary limits
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const limits      &lm = eq.find_primary_limits(Corg);
                const size_t       ii = *eq;
                double            &xx = xi[ii];
                
                YACK_CHEM_PRINTLN("//   @" << ios::align(eq.name,eqs.width) << " : " << std::setw(14) << xx);
                YACK_CHEM_PRINTLN("//   |_" << lm);
                xx = lm.crop(xx);
            }
            YACK_CHEM_PRINTLN("  xi     = " << xi);
            YACK_CHEM_PRINTLN("//   <computeExtent/>");

        }
    }

}


