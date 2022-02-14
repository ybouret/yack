#include "yack/chem/plexus.hpp"
#include "yack/math/tao/v1.hpp"


namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool plexus:: regularize()
        {
            YACK_CHEM_PRINTLN("// [regularizing]");
            bool moved = false;
            blocked.free();
            for(const enode *node=eqs.head();node;node=node->next)
            {
                //--------------------------------------------------------------
                // check acceptable gradient
                //--------------------------------------------------------------
                const equilibrium      &eq  = ***node;
                const size_t            i   = eq.indx;
                const readable<double> &psi = Psi[i];

                //--------------------------------------------------------------
                // check acceptable gradient
                //--------------------------------------------------------------
                if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-1

                moved = true;
                YACK_CHEM_PRINTLN("// |_[moving " << eq.name << "]");
                eq.solve(K[i],Corg,Ctry);
                computeGammaAndPsi(Corg);

                //--------------------------------------------------------------
                // check new acceptable gradient
                //--------------------------------------------------------------
                if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-2

                //--------------------------------------------------------------
                // register as blocked
                //--------------------------------------------------------------
                blocked << i;
            }
            return moved;
        }

    }

}

