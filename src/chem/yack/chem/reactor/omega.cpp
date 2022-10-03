
#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {


        void reactor:: buildOmega0()
        {
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium      &eq  = ***node;
                const size_t            ei  = *eq;
                writable<double>       &Omi = Omega[ei];
                writable<double>       &psi = Psi[ei];
                if(blocked[ei])
                {
                    Omi.ld(0);
                    psi.ld(0);
                    Omi[ei]   = 1.0;
                    Gamma[ei] = 0.0;
                }
                else
                {
                    const double      Ki  = K[ei];
                    eq.grad_action(psi,Ki,Corg,xmul);
                    Gamma[ei] = fabs(Xl[ei])<=0 ? 0 : eq.mass_action(Ki,Corg,xmul);
                    for(const enode *scan=singles.head();scan;scan=scan->next)
                    {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej]);
                    }
                }
            }


          
        }

    }

}

