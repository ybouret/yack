
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

        bool reactor:: initializeSearch(const readable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::initializeSearch");
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);

            //------------------------------------------------------------------
            //
            //
            // checking cases
            //
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    //----------------------------------------------------------
                    // empty
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN(" <success::empty/>");
                    return true;


                case 1:
                    //----------------------------------------------------------
                    // only 1
                    //----------------------------------------------------------
                {
                    const equilibrium &eq = ***singles.head();
                    //eq.solve1D(K[*eq],C0,Corg);
                    (void) eq.brew1D(K[*eq], C0, Corg);
                    YACK_CHEM_PRINTLN(vpfx << "  <success::1D/> " << Corg);
                } return true;

                default:
                    //----------------------------------------------------------
                    // initialize for consistency
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }

            assert(N>0);
            return false;
        }

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
            nrun                    = 0;
            const equilibrium *emax = 0;
            double             xmax = 0;
            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                const double         Ki  = K[ei];
                writable<double>    &Ci  = Cl[ei];
                writable<double>    &psi = Psi[ei];
                writable<double>    &Omi = Omega0[ei];
                const readable<int> &NuI = Nu[ei];
                const outcome        res = eq.brew1D(Ki,Corg,Ci);
                const double         ax  = fabs(Xl[ei] = res.xi0 );
                if(verbose) singles.pad(std::cerr << vpfx << eq.name,eq) << " | " << res << std::endl;

                Omi.ld(0); Omi[ei] = 1.0;

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
                        eq.drvs_action(psi,Ki,Ci,Ctry);    //! psi @ Ceq
                        sigma[ei] = addops->dot(psi,NuI);
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
            // First Pass: probe singles
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

            const bool couldDecreaseLattice = (NULL!=emin);
            YACK_CHEM_PRINTLN(vpfx << "  [couldDreceaseLattice=" << yack_boolean(couldDecreaseLattice) << "]");


            // prepare coupled step
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                double              &gam = Gamma[ei];

                if(blocked[ei])
                {
                    gam = 0;
                }
                else
                {
                    const double         Ki  = K[ei];
                    writable<double>    &psi = Psi[ei];
                    writable<double>    &Omi = Omega0[ei];
                    gam = eq.grad_action(psi,Ki,Corg,Ctry);
                    if(fabs(Xl[ei])<=0) gam = 0;
                    for(const enode *scan=singles.head();scan;scan=scan->next)
                    {
                        const size_t ej = ****scan;
                        if(ej!=ei)
                        {
                            Omi[ej] = - addops->dot(Nu[ej],psi)/sigma[ei];
                        }
                    }
                }

            }

            singles(std::cerr << vpfx << "Omega=", Omega0, vpfx);
            singles(std::cerr << vpfx << "Psi  =", Psi,    vpfx);
            singles(std::cerr << vpfx << "Nu   =", Nu,     vpfx);
            singles(std::cerr << vpfx << "Xi   =", Xl,     vpfx);

            iOmega.assign(Omega0);
            if(!LU->build(iOmega))
            {
                YACK_CHEM_PRINTLN("  <singular>");
                return false;
            }

            iota::load(xi,Xl);
            LU->solve(iOmega,xi);
            singles(std::cerr << vpfx << "xi   =", xi, vpfx);


            {
                bool overshoot = false;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const double       xx = xi[ei];

                    //----------------------------------------------------------
                    //
                    // check extent for equilibrium
                    //
                    //----------------------------------------------------------
                    if(verbose) singles.pad(std::cerr << "@{" << eq.name << "}",eq) << " : ";

                    if(blocked[ei])
                    {
                        // won't move anyway
                        if(verbose) std::cerr << "[blocked]";
                    }
                    else
                    {
                        const limits &lm = eq.primary_limits(Corg,lib.width);
                        if( lm.should_reduce(xx) )
                        {
                            overshoot = true;
                            YACK_CHEM_PRINT("[REJECT]");
                            zapEquilibriumAt(ei);
                            --nrun;
                        }
                        else
                        {
                            YACK_CHEM_PRINT("[accept]");
                        }
                        YACK_CHEM_PRINT( " " << std::setw(15) << xx << " | " << lm) ;
                    }
                    if(verbose) std::cerr << std::endl;
                }

                //--------------------------------------------------------------
                //
                // at least one overshoot!
                //
                //--------------------------------------------------------------
                if(overshoot)
                {
                    return false;
                }

            }






            return true;
        }
    }

}

