
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        size_t reactor:: computeOmegaAndGamma() throw()
        {
            YACK_CHEM_MARKUP("//    ","plexus::omega_and_gamma");
            coerce(NuTA).assign(NuT);
            size_t num_blocked = 0;
            size_t num_running = N;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega0[ei];
                const double       Ki  = K[ei];
                const double       ax  = fabs( Xtot[ei] = eq.solve1D(Ki,Corg,Ctmp) );

                Omi.ld(0);
                if(verbose)
                {
                    eqs.pad(std::cerr << vpfx << eq.name,eq) << " : Gamma=";
                }
                if(ax<=0)
                {
                    Gamma[ei] = 0;
                    eq.drvs_action(Psi[ei],Ki,Corg,Ctmp);
                }
                else
                {
                    Gamma[ei] = eq.grad_action(Psi[ei], Ki, Corg, Ctmp);
                }

                if(verbose)
                {
                    std::cerr << std::setw(15) << Gamma[ei];
                }

                const double diag = sorted::dot(psi,Nu[ei],Ctmp);
                if( diag >= 0)
                {
                    ++num_blocked;
                    --num_running;
                    if(verbose) std::cerr << " [blocked]";
                    Omi[ei]     = 1.0;
                    Gamma[ei]   = 0.0;
                    blocked[ei] = true;
                    psi.ld(0);
                    coerce(NuTA).ld_col(ei,0);
                }
                else
                {
                    if(verbose) std::cerr << " [running] " << psi;
                    Omi[ei] = -diag;
                    for(size_t k=N;k>ei;--k)   Omi[k] = -sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = -sorted::dot(psi,Nu[k],Ctmp);
                    blocked[ei] = false;
                }

                if(verbose) std::cerr << std::endl;

            }
            if(verbose)
            {
                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "blocked=" << blocked << std::endl;
                std::cerr << "NuT =" << NuT << std::endl;
                std::cerr << "NuTA=" << NuTA << std::endl;
            }
            return num_running;
        }
    }

}


