
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
#if 0
        void reactor:: updateOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::updateOmega0");

            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium       &eq = ***node;
                const size_t             ei = *eq;
                writable<double>        &psi = Psi[ei];
                writable<double>        &Omi = Omega0[ei];
                double                  &gam = Gamma[ei];

                if(blocked[ei])
                {
                    assert(fabs(sigma[ei])<=0);
                    gam = 0;
                    Omi.ld(0); Omi[ei]=1.0;
                    NuA[ei].ld(0);
                    psi.ld(0);
                }
                else
                {
                    assert(sigma[ei]>0);
                    gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                    if( fabs(Xl[ei]) <= 0 )  gam = 0;

                    for(const enode *scan=singles.head();scan;scan=scan->next)
                    {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                }
            }

            if(verbose)
            {
                singles(std::cerr << vpfx << "Omega=",Omega0,vpfx);
                singles(std::cerr << vpfx << "Gamma=",Gamma,vpfx);
                std::cerr << "Psi=" << Psi << std::endl;
                std::cerr << "Nu=" << Nu << std::endl;
                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "Psi*Nu'" << std::endl;

            }
            //exit(1);


        }
#endif
        
    }

}


