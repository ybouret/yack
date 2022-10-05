
#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"


namespace yack
{
    using namespace math;

    namespace chemical
    {



        const equilibrium * reactor:: getTopology(size_t  &nrun, const xmlog &xml)
        {
            nrun                    = 0;      // running eqs
            double             amax = 0;      // max absolute solving extent
            const equilibrium *emax = NULL;   // whose extent it is
            outcome            ppty;          // local default outcome
            NuA.assign(Nu);                   // initial: full run

            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Ceq[ei];
                const double       Ki  = K[ei];
                const outcome      oc  = outcome::study(eq, Ki, Corg, Ci, xmul, xadd);
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega[ei];

                Omi.ld(0);

                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        Xl[ei]      = 0;
                        sigma[ei]   = 0;
                        NuA[ei].ld(0);
                        psi.ld(0);
                        Omi[ei]     = 1;
                        break;

                    case components::are_running: {
                        ++nrun;
                        blocked[ei] = false;
                        const double ax = fabs( Xl[ei] = oc.value );
                        if(ax>amax)
                        {
                            amax =  ax;
                            emax = &eq;
                            ppty =  oc;
                        }
                        eq.grad_action(psi,Ki,Ci,xmul);
                        sigma[ei] = xadd.dot(psi, Nu[ei]);
                        if(sigma[ei]>=0) throw imported::exception(clid,"corrupted <%s>",eq.name());
                    } break;
                }

                if(verbose) {
                    singles.pad(*xml << "(+) " << '<' << eq.name << '>', eq) << " : " << oc << " @sigma= " << sigma[ei] << std::endl;
                }
            }

            if(emax)
            {
                if(extent::is_degenerated==ppty.grade)
                    return NULL;
                else
                    return emax;
            }
            else
            {
                return NULL;
            }


        }

    }

}


