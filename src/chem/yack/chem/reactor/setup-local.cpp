
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
       
        void reactor:: updateOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::solve::updateOmega0");

            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium       &eq = ***node;
                const size_t             ei = *eq;
                const readable<double>  &psi = Psi[ei];
                writable<double>        &Omi = Omega0[ei];
                double                  &gam = Gamma[ei];

                if(blocked[ei])
                {
                    assert(fabs(sigma[ei])<=0);
                    gam = 0;
                    Omi.ld(0); Omi[ei]=1.0;
                    NuA[ei].ld(0);
                }
                else
                {
                    assert(sigma[ei]>0);
                    gam     = (fabs(Xl[ei]) <= 0) ? 0 : eq.mass_action(K[ei],Corg);
                    Omi[ei] = sigma[ei];

                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                }
            }

            if(verbose)
            {
                singles(std::cerr << vpfx << "Omega=",Omega0,vpfx);
                singles(std::cerr << vpfx << "Gamma=",Gamma,vpfx);

            }



        }
        
    }

}


