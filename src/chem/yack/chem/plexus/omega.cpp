
#include "yack/chem/plexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        size_t plexus:: computeOmega() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeOmega");
            size_t num_running = N;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                writable<double>    &psi = Psi[ei];
                writable<double>    &Omi = Omega0[ei];
                writable<int>       &nuA = NuA[ei];
                const readable<int> &nui = Nu[ei];
                double              &Gam = Gamma[ei];

                Gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                const double d = sorted::dot(psi,nui,Ctry);
                if(d>=0)
                {
                    --num_running;
                    Omi.ld(0);
                    nuA.ld(0);
                    Omi[ei]     = 1.0;
                    Gam         = 0.0;
                    blocked[ei] = true;
                }
                else
                {
                    if( fabs(Xl[ei]) <= 0) Gam = 0.0; // regularize a little
                    iota::load(nuA,nui);
                    blocked[ei] = false;
                    Omi[ei]     = -d;
                    for(const enode *scan=node->prev;scan;scan=scan->prev)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                    for(const enode *scan=node->next;scan;scan=scan->next)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                }
            }

            if(verbose) singles(std::cerr << vpfx << "Omega=",Omega0,vpfx);
            return num_running;
        }

        void   plexus:: suspendEquilibriumAt(const size_t ei) throw()
        {
            assert(!blocked[ei]);
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            NuA[ei].ld(0); // set inactive
            Xl[ei]    = 0; // block Xi
            Gamma[ei] = 0; // block Gamma
            Omi.ld(0);     // set inactive Omega
            Omi[ei] = 1.0; // regularize
        }
    }

}


