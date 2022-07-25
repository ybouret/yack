
#include "yack/chem/reactor.hpp"
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

        bool reactor:: successfulUpdate(writable<double> &C0, unsigned int cycle) throw()
        {
            active.transfer(C0,Corg);

            std::cerr << "  ---------------- leave #" << cycle << " ----------------" << std::endl;
            lib(std::cerr << "Cend=",C0);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const outcome      res = eq.brew1D(K[*eq],C0,Ctry);
                singles.pad(std::cerr << eq.name,eq);
                std::cerr << " | " << res;
                std::cerr << " | ma=" << std::setw(15) << eq.mass_action(K[*eq],C0);
                std::cerr << " |  Q/K-1=" << std::setw(15) << (eq.Q(C0)/K[*eq])-1;
                std::cerr << std::endl;
            }


            return true;
        }


        const equilibrium *reactor:: maximumOfSingles(size_t &nrun) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::maximumOfSingles");
            nrun = 0;
            const equilibrium *emax = 0;
            double             xmax = 0;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                const double         Ki  = K[ei];
                writable<double>    &Ci  = Cl[ei];
                writable<double>    &psi = Psi[ei];
                const readable<int> &NuI = Nu[ei];
                const outcome        res = eq.brew1D(Ki,Corg,Ci);
                const double         ax  = fabs(Xl[ei] = res.xi0 );
                if(verbose) singles.pad(std::cerr << vpfx << eq.name,eq) << " | " << res << std::endl;

                switch(res.are)
                {
                    case blocked_components:
                        blocked[ei] = true;
                        psi.ld(0);
                        NuA[ei].ld(0);
                        sigma[ei] = 0;
                        break;

                    case running_components:
                        blocked[ei] = false;
                        iota::load(NuA[ei],NuI);
                        eq.drvs_action(psi,Ki,Ci,Ctry);
                        sigma[ei] = - nws.dot(psi,NuI,Ctry);
                        ++nrun;
                        if(ax>xmax)
                        {
                            xmax=ax;
                            emax=&eq;
                        }

                        break;
                }
            }

            YACK_CHEM_PRINTLN(   vpfx << "  #run=" << nrun);
            singles(std::cerr << vpfx << "    sigma=",sigma,vpfx);
            singles(std::cerr << vpfx << "    blocked=",blocked,vpfx);

            if(!emax)
            {
                // may be all blocked or at equilibrium
                YACK_CHEM_PRINTLN("|Xi|=0");
                return NULL;
            }
            else
            {
                // not at numerical equilibrium
                assert(emax);
                YACK_CHEM_PRINTLN("|Xi|=" << xmax << " @" << emax->name);
                return emax;
            }
        }


        const equilibrium  * reactor::minimumOfLattice(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::minimumOfLattice");
            double             gmin = G0;
            const equilibrium *emin = NULL;

            // singles have already their eq precomputed
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                if(blocked[ei]) continue;

                writable<double>  &Ci  = Cl[ei];
                iota::load(Cend,Ci);

                const double      gtry = Htry(G0);
                const bool        good = (gtry<gmin);

                YACK_CHEM_PRINTLN(vpfx << accepting(good) << "G=" << std::setw(15) << gtry << " @" << eq.name);
                if(good)
                {
                    gmin = gtry;
                    emin = &eq;
                }

            }

            // full computation for couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = Kl[ei];
                const outcome      res = eq.brew1D(Ki,Corg,Cend);

                switch(res.are)
                {
                    case blocked_components:
                        YACK_CHEM_PRINTLN(vpfx << "(blocked { " << eq.name << " })");
                        blocked[ei] = true;
                        iota::load(Cl[ei],Corg);
                        break;

                    case running_components: {
                        blocked[ei] = false;
                        const double      gtry = Htry(G0);
                        const bool        good = (gtry<gmin);
                        iota::load(Cl[ei],Ctry);
                        YACK_CHEM_PRINTLN(vpfx << accepting(good) << "G=" << std::setw(15) << gtry << " @" << eq.name);
                        if(good)
                        {
                            gmin = gtry;
                            emin = &eq;
                        }

                    } break;
                }


            }

            if(verbose)
            {
                if(emin)
                {
                    YACK_CHEM_PRINTLN(vpfx << " Gmin =" << std::setw(15) << gmin << " @" << emin->name << " <-- ");
                }
            }

            return emin;
        }


        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // workspace initialization
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0; //ios::ocstream::overwrite("g0.dat");
            if( initializeSearch(C0) )
            {
                return successfulUpdate(C0,cycle); // fastest !
            }


        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("  ---------------- cycle #" << cycle << " ----------------");
            if(verbose)
            {
                lib(std::cerr << vpfx << "Corg=",Corg,vpfx);
            }

            //------------------------------------------------------------------
            //
            //
            // probe singles
            //
            //
            //------------------------------------------------------------------
            size_t nrun = 0;
            {
                const equilibrium *emax = maximumOfSingles(nrun);

                if( !emax )
                    return successfulUpdate(C0,cycle);

                switch(nrun)
                {
                    case 0: // should already be processed...
                        return successfulUpdate(C0,cycle);

                    case 1: // emax is the only one!
                        (void) emax->brew1D(K[**emax],Corg,Ctry);
                        active.transfer(Corg,Ctry);
                        goto CYCLE;

                    default: // generic case
                        break;
                }
                assert(nrun>=2);
                assert(emax);
            }

            //------------------------------------------------------------------
            //
            //
            // initialize G0 using the first pass topology
            //
            //
            //------------------------------------------------------------------
            double G0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN("G0="<<G0);


            //------------------------------------------------------------------
            //
            //
            // find the minimizing equilibrium within lattice from G0
            //
            //
            //------------------------------------------------------------------
            const equilibrium *emin = minimumOfLattice(G0);

            if(emin)
            {
                YACK_CHEM_PRINTLN(vpfx << "minimized by {" << emin->name << "}");

                //--------------------------------------------------------------
                //
                // check other degrees of freedom
                //
                //--------------------------------------------------------------
                const double G1 = buildHamiltonian(*emin);
                if(verbose)
                {
                    lib(std::cerr << vpfx << "Cmin=",Corg,vpfx);
                }

                if(G1<=0)
                {
                    // early return
                    return successfulUpdate(C0,cycle);
                }

                //--------------------------------------------------------------
                //
                // recompute topology
                //
                //--------------------------------------------------------------
                if(!maximumOfSingles(nrun))
                {
                    // early return
                    return successfulUpdate(C0,cycle);
                }
            }
            else
            {
                YACK_CHEM_PRINTLN(vpfx << "already at lattice minimum");
            }




            exit(1);

            return false;
        }
    }

}

