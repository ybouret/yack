
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
        const equilibrium * reactor:: getDominant(double &Hmin)
        {
            const equilibrium *emin = NULL;
            const double       H0   = Hmin;

            //------------------------------------------------------------------
            //
            //
            // look within singles, Ceq is already computed
            //
            //
            //------------------------------------------------------------------
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;              if(blocked[ei])  continue;
                writable<double>  &Ci = Ceq[ei];          working.transfer(Cend,Ci);
                const double       H1 = Optimized1D(H0);  working.transfer(Ci,Ctry);
                const bool         ok = (H1<Hmin);
                if(ok)
                {
                    Hmin = H1;
                    emin = &eq;
                }

                if(verbose)
                {
                    lattice.pad(std::cerr << "    @" << eq.name,eq) << " -> " << std::setw(15) << H1;
                    if(ok) std::cerr << " <-- :)";
                    std::cerr << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // look within couples
            //
            //
            //------------------------------------------------------------------
            for(const enode *node = couples.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const outcome      oc = outcome::study(eq, Kl[ei], Corg, Cend, xmul, xadd);
                switch(oc.state)
                {
                    case components::are_blocked:
                        Xl[ei] = 0;
                        working.transfer(Ceq[ei],Corg);
                        break;

                    case components::are_running: {
                        Xl[ei] = oc.value;
                        const double H1 = Optimized1D(H0);working.transfer(Ceq[ei],Ctry);
                        const bool   ok = (H1<Hmin);
                        if(ok)
                        {
                            Hmin = H1;
                            emin = &eq;
                        }

                        if(verbose)
                        {
                            lattice.pad(std::cerr << "    @" << eq.name,eq) << " -> " << std::setw(15) << H1;
                            if(ok) std::cerr << " <-- :)";
                            std::cerr << std::endl;
                        }
                    } break;
                }
            }

            return emin;
        }
    }

}

