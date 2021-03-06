#include "yack/chem/plexus.hpp"
#include "yack/math/tao/v1.hpp"


namespace yack
{
    using namespace math;

    namespace chemical
    {
        size_t plexus:: regularize()
        {
            YACK_CHEM_PRINTLN("//" << std::endl << "//   <regularizing>");
            assert(N==blocked.size());
            size_t moved = 0;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                //--------------------------------------------------------------
                // access gradient
                //--------------------------------------------------------------
                const equilibrium      &eq  = ***node;
                const size_t            i   = *eq;
                const readable<double> &psi = Psi[i];
                blocked[i] = false;

                //--------------------------------------------------------------
                // check acceptable gradient
                //--------------------------------------------------------------
                if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-1


                //--------------------------------------------------------------
                // move and update
                //--------------------------------------------------------------
                ++moved;
                YACK_CHEM_PRINTLN("//   |_[moving " << eq.name << "]");
                eq.solve(K[i],Corg,Ctmp);
                computeGammaAndPsi(Corg);

                //--------------------------------------------------------------
                // check new acceptable gradient
                //--------------------------------------------------------------
                if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-2

                //--------------------------------------------------------------
                // register as blocked
                //--------------------------------------------------------------
                blocked[i] = true;
            }
            YACK_CHEM_PRINTLN("//     moved=" << moved << ", blocked=" << blocked);
            YACK_CHEM_PRINTLN("//   <regularizing/>");
            return moved;
        }

    }

}

