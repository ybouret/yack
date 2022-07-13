
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        void   reactor:: zapEquilibriumAt(const size_t ei) throw()
        {
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            sigma[ei]   = 0;
            Gamma[ei]   = 0;
            Omi.ld(0); Omi[ei] = 1.0;
            NuA[ei].ld(0);
        }

        size_t reactor:: initializeOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::initializeLocalStep");

            size_t num_running = N;
            NuA.assign(Nu);

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega0[ei];
                double            &gam = Gamma[ei];

                gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                const double den = - sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    blocked[ei] = true;
                    Omi.ld(0); Omi[ei] = 1.0;
                    gam=0;
                    NuA[ei].ld(0);
                    --num_running;
                }
                else
                {
                    blocked[ei] = false;
                    Omi[ei]     = den;
                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                    if( fabs(Xl[ei]) <= 0 ) gam = 0;
                }

            }
            if(verbose)
            {
                singles(std::cerr<<vpfx<<"blocked=",blocked,vpfx);
                singles(std::cerr<<vpfx<<"Omega=",Omega0,vpfx);
                singles(std::cerr<<vpfx<<"Gamma=",Gamma,vpfx);
            }

            return num_running;

        }

    }

}


