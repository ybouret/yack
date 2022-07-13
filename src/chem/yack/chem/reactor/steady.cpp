
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


      

       
#if 0
        bool    reactor:: steady(writable<double> &C0) throw()
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::computeSteady");
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);

            ios::ocstream::overwrite("g.dat");

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
                    YACK_CHEM_PRINTLN(" <success::empty/>");
                    return true;

                case 1: {
                    const equilibrium &eq = ***singles.head();
                    eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(" <success::1D/>");
                } return returnSuccessful(C0,0);

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }

            assert(N>0);

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            //
            // initialize singles
            //
            //
            //------------------------------------------------------------------
            double             Xmax = 0;
            const equilibrium *emax = NULL;
            size_t             nrun = 0;

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = Kl[ei];
                writable<double>  &Ci  = Cl[ei];
                writable<double>  &psi = Psi[ei];
                const double       xx  = ( Xl[ei] = eq.solve1D(Ki,Corg,Ci) );
                const double       ax  = fabs(xx);

                if(ax>Xmax)
                {
                    Xmax = ax;
                    emax = &eq;
                }

                eq.drvs_action(psi,Ki,Ci,Ctry);
                const double den = -sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    sigma[ei]   = 0.0;
                    blocked[ei] = true;
                    Xl[ei]      = 0.0;
                }
                else
                {
                    sigma[ei]   = den;
                    blocked[ei] = false;
                    ++nrun;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // check single status
            //
            //
            //------------------------------------------------------------------
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xl,vpfx);
            if(emax)
            {
                assert(Xmax>0);
                YACK_CHEM_PRINTLN(vpfx << "|Xmax|=" << Xmax << " @{" << emax->name << "}");
            }
            else
            {
                assert(Xmax<=0);
                YACK_CHEM_PRINTLN("  <success:: |Xi| = 0/>");
                return returnSuccessful(C0,cycle);
            }



            //------------------------------------------------------------------
            //
            //
            // check running status
            //
            //
            //------------------------------------------------------------------
            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN("  <success::all-blocked/>");
                    return returnSuccessful(C0,cycle);

                case 1:
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        if(!blocked[ei])
                        {
                            active.transfer(Corg,Cl[ei]);
                            goto CYCLE;
                        }
                    }
                    YACK_CHEM_PRINTLN("  <failure::corrupted-1D>");
                    return false;
            }

            //------------------------------------------------------------------
            //
            //
            // compute initial global hamiltonian
            //
            //
            //------------------------------------------------------------------
            double G0 = Hamiltonian(Corg);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN("  <success:: Hamiltonian=0/>");
                return returnSuccessful(C0,cycle);
            }

            ios::ocstream::echo("g.dat", "%u %.15g\n",cycle,log10(G0));
            YACK_CHEM_PRINTLN(vpfx << " G0 = " << G0);

            double             Gmin = G0;
            const equilibrium *emin = NULL;

            //------------------------------------------------------------------
            //
            // minimize singles using precomputed Cl[] = Ceq
            //
            //------------------------------------------------------------------
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Cl[ei];
                iota::load(Cend,Ci);
                const double Gtry = Htry(G0);
                if(Gtry<Gmin)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                iota::load(Ci,Ctry);

                lattice.pad(std::cerr << vpfx << "@{" << eq.name << "}",eq) << " : " << std::setw(15) << Gtry << std::endl;

            }

            //------------------------------------------------------------------
            //
            // minimize couples
            //
            //------------------------------------------------------------------
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                (void)eq.solve1D(Kl[ei],Corg,Cend);
                const double Gtry = Htry(G0);
                if(Gtry<Gmin)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                iota::load(Cl[ei],Ctry);
                lattice.pad(std::cerr << vpfx << "@{" << eq.name << "}",eq) << " : " << std::setw(15) << Gtry << std::endl;
            }



            if(emin)
            {
                std::cerr << "Gmin=" << Gmin << " @" << emin->name << std::endl;
                std::cerr << "Hmin=" << Hamiltonian(Cl[**emin]) << std::endl;
                assert(Gmin<G0);
                const double G1    = buildHamiltonian(*emin); assert(G1<=Gmin);
                const double Delta = fabs(G0-G1);
                std::cerr << "Delta=" << Delta << std::endl;
                G0 = G1;
                lib(std::cerr << vpfx << "Cnew=",Corg,vpfx);

                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    lattice.pad(std::cerr << "@{" << eq.name << "}",eq);
                    const double ma = eq.mass_action(K[ei],Corg);
                    std::cerr << ": ma = " <<  std::setw(15) << ma;
                    const double sig = sigma[ei];
                    std::cerr << " / " <<  std::setw(15) << sig;
                    if(!blocked[ei])
                        std::cerr << " => +" << fabs(ma/sig);
                    std::cerr << std::endl;
                }

                if(G0<=0)
                {
                    YACK_CHEM_PRINTLN("  <success:: Hamiltonian=0/>");
                    return returnSuccessful(C0,cycle);
                }

                ios::ocstream::echo("g.dat", "%u.5 %.15g\n",cycle,log10(G0));

#if 0
                static const double gtol = sqrt( numeric<double>::epsilon );
                if( Delta <= gtol * G0 )
                {
                    std::cerr << "Minimum Delta!!" << std::endl;
                }
                else
                {
                    goto CYCLE;
                }
#endif

#if 0
                // update singles
                Xmax = 0;
                nrun = 0;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    const double       Ki  = Kl[ei];
                    writable<double>  &Ci  = Cl[ei];
                    writable<double>  &psi = Psi[ei];
                    const double       xx  = ( Xl[ei] = eq.solve1D(Ki,Corg,Ci) );
                    const double       ax  = fabs(xx);

                    if(ax>Xmax)
                    {
                        Xmax = ax;
                    }

                    eq.drvs_action(psi,Ki,Ci,Ctry);
                    const double den = -sorted::dot(psi,Nu[ei],Ctry);
                    if(den<=0)
                    {
                        sigma[ei]   = 0.0;
                        blocked[ei] = true;
                    }
                    else
                    {
                        sigma[ei]   = den;
                        blocked[ei] = false;
                        ++nrun;
                    }
                }
                singles(std::cerr << vpfx << "sigma=",sigma,vpfx);
#endif

                // recompute scaling of the best location
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    const double       Ki  = Kl[ei];
                    writable<double>  &Ci  = Cl[ei];
                    Xl[ei] = eq.solve1D(Ki,Corg,Ci);
                }

            }
            else
            {
                std::cerr << "No Better Than " << G0 << std::endl;
            }

            //------------------------------------------------------------------
            //
            //
            // Local Step
            //
            //
            //------------------------------------------------------------------
            {
                //--------------------------------------------------------------
                //
                // initialize maximum system
                //
                //--------------------------------------------------------------
                YACK_CHEM_MARKUP(vpfx, "reactor::computeExtent");
                bool      maximumAvailableDOF = true;
                size_t    num_running         = initializeOmega0();
                unsigned  inner               = 0;

            COMPUTE_EXTENT:
                ++inner;
                YACK_CHEM_PRINTLN(vpfx << " -------- extent " << cycle << "." << inner << " --------");
                //--------------------------------------------------------------
                //
                // process cases
                //
                //--------------------------------------------------------------
                switch(num_running)
                {
                    case 0:
                        YACK_CHEM_PRINTLN("  <failure::all-blocked>");
                        return false;

                    case 1:
                        for(const enode *node=singles.head();node;node=node->next)
                        {
                            const equilibrium &eq = ***node;
                            const size_t       ei = *eq;
                            if(!blocked[ei])
                            {
                                (void) eq.solve1D(K[ei],Corg,Ctry);
                                active.transfer(Corg,Ctry);
                                G0 = Hamiltonian(Corg);
                                goto CYCLE;
                            }
                        }
                        YACK_CHEM_PRINTLN("  <failure::corrupted-1D>");
                        return false;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                //
                // compute extent
                //
                //--------------------------------------------------------------
                iOmega.assign(Omega0);
                if(!LU->build(iOmega))
                {
                    YACK_CHEM_PRINTLN("  <singular local system>");
                    goto CYCLE; // try another global step ??
                }

                iota::load(xi,Gamma);
                LU->solve(iOmega,xi);

                //--------------------------------------------------------------
                //
                // validate primary constraints
                //
                //--------------------------------------------------------------
                {
                    bool overshoot = false;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        const double       xx = xi[ei];

                        if(verbose) singles.pad(std::cerr << "@{" << eq.name << "}",eq) << " : ";
                        if(blocked[ei])
                        {
                            if(verbose) std::cerr << "[blocked]";
                        }
                        else
                        {
                            const limits &lm = eq.primary_limits(Corg,lib.width);
                            if( lm.should_reduce(xx) )
                            {
                                overshoot = true;
                                --num_running;
                                YACK_CHEM_PRINT("[REJECT]");
                                zapEquilibriumAt(ei);
                            }
                            else
                            {
                                YACK_CHEM_PRINT("[accept]");
                            }
                            YACK_CHEM_PRINT( " " << std::setw(15) << xx << " | " << lm) ;
                        }
                        if(verbose) std::cerr << std::endl;
                    }

                    if(overshoot)
                    {
                        maximumAvailableDOF = false;
                        goto COMPUTE_EXTENT;
                    }
                }
                YACK_CHEM_PRINTLN(vpfx << "    [maximumAvailableDOF]=" << yack_boolean(maximumAvailableDOF) );

                if(!optimizeFullStep(G0))
                {
                    YACK_CHEM_PRINTLN(vpfx << "    |_[minimum hamiltonian]");
                    YACK_CHEM_PRINTLN(vpfx << "    |_[maximumAvailableDOF]=" << yack_boolean(maximumAvailableDOF) );
                    
                    (void) returnSuccessful(C0,cycle);

                    exit(1);


                }
                else
                {
                    goto CYCLE;
                }


            }

            return false;
        }
#endif
        
    }

}

