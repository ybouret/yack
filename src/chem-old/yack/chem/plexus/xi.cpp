

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

        void plexus:: computeExtent()
        {
            YACK_CHEM_PRINTLN("//" << std::endl <<  "//   <computeExtent>");

            //------------------------------------------------------------------
            //
            // compute predicted extent
            //
            //------------------------------------------------------------------
            tao::v3::mmul_trn(W,Psi,Nu);
            for(size_t i=N;i>0;--i)
            {
                if( blocked[i] )
                {
                    W[i][i]  = 1.0;
                    Gamma[i] = 0.0;
                }
            }
            YACK_CHEM_PRINTLN("//    W     = " << W);
            if(!LU.build(W)) throw exception("%s   singular composition",clid);
            tao::v1::neg(xi,Gamma);
            LU.solve(W,xi);
            YACK_CHEM_PRINTLN("//     xi_p   = " << xi);


            //------------------------------------------------------------------
            //
            // compute corrected extent
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//     <correcting>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const limits      &lm = eq.find_primary_limits(Corg);
                const size_t       ei = *eq;
                if(verbose)
                {
                    eqs.pad(std::cerr << "//       @" << eq.name,eq.name) << " : " << std::setw(14) << xi[ei] << " | " << lm << std::endl;
                }
                xi[ei] = lm.crop(xi[ei]);
            }
            YACK_CHEM_PRINTLN("//     <correcting/>");
            YACK_CHEM_PRINTLN("//     xi_c   = " << xi);
            YACK_CHEM_PRINTLN("//     Gamma  = " << Gamma);
            YACK_CHEM_PRINTLN("//     xs     = " << xs);
            YACK_CHEM_PRINTLN("//   <computeExtent/>");

        }

    }

}


