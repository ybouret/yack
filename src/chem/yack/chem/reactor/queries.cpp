
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/numeric.hpp"
#include <iomanip>
#include <cmath>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        bool  reactor:: querySingles(size_t &nrun) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::querySingles");
            double             Xmax = 0;
            const equilibrium *emax = NULL;
            nrun                    = 0;

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                writable<double>  &Ci = Cl[ei];
                const double       xx = (Xl[ei]=eq.solve1D(Ki,Corg,Ci));
                const double       ax = fabs(xx);
                if(ax>Xmax)
                {
                    Xmax =  ax;
                    emax = &eq;
                }
                writable<double> &psi = Psi[ei];
                eq.drvs_action(psi,Ki,Ci,Ctry);
                const double den = - sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    blocked[ei] = true;
                    sigma[ei]   = 0.0;
                }
                else
                {
                    blocked[ei] = false;
                    sigma[ei]   = den;
                    ++nrun;
                }

                if(verbose)
                {
                    singles.pad(std::cerr << vpfx << "  @{" << eq.name << "}",eq);
                    std::cerr << " | xi = " << std::setw(15) << xx;
                    std::cerr << " | sigma = " << std::setw(15) << sigma[ei];
                    if(blocked[ei])
                        std::cerr << " | blocked";
                    else
                        std::cerr << " | running";
                    std::cerr << std::endl;
                }

            }

            YACK_CHEM_PRINTLN(vpfx << "  #run = " << nrun);
            if(!emax)
            {
                assert(Xmax<=0);
                YACK_CHEM_PRINTLN(vpfx << "  [success : |Xi| = 0 ]");
                return true;
            }
            else
            {
                YACK_CHEM_PRINTLN(vpfx << "  |Xi| = " << Xmax << " @{" << emax->name << "}" );
                return false;
            }
        }



        const equilibrium *reactor:: queryLattice(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::queryLattice");

            double             Gmin = G0;
            const equilibrium *emin = NULL;

            //------------------------------------------------------------------
            //
            // setup from precomputed singles
            //
            //------------------------------------------------------------------
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq   = ***node;
                const size_t       ei   = *eq;
                writable<double>  &Ci   = Cl[ei];   iota::load(Cend,Ci);
                const double       Gtry = Htry(G0); iota::load(Ci,Ctry);
                const bool         good = Gtry<Gmin;
                if(good)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                YACK_CHEM_PRINTLN(vpfx << accepting(good) << std::setw(15) << Gmin << " @{" << eq.name << "}");

            }

            // setup from couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq   = ***node;
                const size_t       ei   = *eq;
                (void) eq.solve1D(Kl[ei],Corg,Cend);
                const double       Gtry = Htry(G0); iota::load(Cl[ei],Ctry);
                const bool         good = Gtry<Gmin;
                if(good)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                YACK_CHEM_PRINTLN(vpfx << accepting(good) << std::setw(15) << Gmin << " @{" << eq.name << "}");
            }

            return emin;
        }


    }

}


