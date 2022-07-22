
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

            //------------------------------------------------------------------
            //
            //
            // probe singles
            //
            //------------------------------------------------------------------
            {
                YACK_CHEM_MARKUP(vpfx, "reactor::studySingles");
                size_t             nrun = 0;
                const equilibrium *emax = 0;
                double             xmax = 0;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    const double       Ki  = K[ei];
                    writable<double>  &Ci  = Cl[ei];
                    writable<double>  &psi = Psi[ei];
                    const outcome      res = eq.brew1D(Ki,Corg,Ci);
                    const double       ax  = fabs(Xl[ei] = res.xi0 );
                    if(verbose) singles.pad(std::cerr << vpfx << eq.name,eq) << " | " << res << std::endl;

                    switch(res.are)
                    {
                        case blocked_components:
                            blocked[ei] = true;
                            psi.ld(0);
                            NuA[ei].ld(0);
                            break;

                        case running_components:
                            blocked[ei] = false;
                            iota::load(NuA[ei],Nu[ei]);
                            eq.drvs_action(psi,Ki,Ci,Ctry);
                            sigma[ei] = - sorted::dot(psi,Nu[ei],Ctry);
                            ++nrun;
                            if(ax>xmax)
                            {
                                xmax=ax;
                                emax=&eq;
                            }

                            break;
                    }
                }

                YACK_CHEM_PRINTLN("#run=" << nrun);
                singles(std::cerr << vpfx << "sigma=",sigma,vpfx);
                singles(std::cerr << vpfx << "blocked=",blocked,vpfx);

                if(!emax)
                {
                    // may be all blocked!
                    return successfulUpdate(C0,cycle);
                }

                assert(emax);
                YACK_CHEM_PRINTLN("|Xi|=" << xmax << " @" << emax->name);


                switch(nrun)
                {
                    case 0:
                        return successfulUpdate(C0,cycle);

                    case 1:
                        (void) emax->brew1D(K[**emax],Corg,Ctry);
                        active.transfer(Corg,Ctry);
                        goto CYCLE;

                    default:
                        break;
                }
                assert(nrun>=2);
            }

            double G0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN("G0="<<G0);

            {
                double             gmin = G0;
                const equilibrium *emin = NULL;
                {
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
                }

                {
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
                }

                if(emin)
                {
                    YACK_CHEM_PRINTLN(vpfx << " Gmin =" << std::setw(15) << gmin << " @" << emin->name << " <-- ");
                    std::cerr << "min @" << emin->name << std::endl;
                    const double G1 = buildHamiltonian(*emin);

                    if(G1<=0)
                    {
                        // ok...
                        return successfulUpdate(C0,cycle);
                    }

                    // recompute topology
                    

                }
                else
                {

                }
            }



            exit(1);

            return false;
        }
    }

}

