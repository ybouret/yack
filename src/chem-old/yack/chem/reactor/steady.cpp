#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/numeric.hpp"

#include <iomanip>
#include <cmath>


#if 0
namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool  reactor:: updateSuccessful(writable<double> &C0, unsigned cycle) throw()
        {
            active.transfer(C0,Corg);
            if(verbose)
            {

                std::cerr << "  ---------------- leave #" << cycle << " ----------------" << std::endl;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    singles.pad(std::cerr << eq.name,eq);
                    std::cerr << " | xi=" << std::setw(15) << eq.solve1D(K[*eq],C0,Ctry);
                    std::cerr << " | ma=" << std::setw(15) << eq.mass_action(K[*eq],C0);
                    std::cerr << " |  Q/K-1=" << std::setw(15) << (eq.Q(C0)/K[*eq])-1;
                    std::cerr << std::endl;
                }
            }
            return true;
        }

       

        void reactor:: initializeOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::initializeOmega0");
            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium       &eq  = ***node;
                const size_t             ei  = *eq;
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
                //std::cerr << "Psi=" << Psi << std::endl;
                //std::cerr << "Nu=" << Nu << std::endl;
                std::cerr << "Omega=" << Omega0 << std::endl;
                //std::cerr << "-Psi*Nu'" << std::endl;
            }


        }


        bool reactor::steady(writable<double> &C0) throw()
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
                return updateSuccessful(C0,cycle); // fastest !
            }



        GLOBAL_STEP:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- cycle " << cycle  << " ----------------");
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);

            //------------------------------------------------------------------
            //
            //
            // compute all single extents and sigma for hamiltonian
            //
            //
            //------------------------------------------------------------------
            size_t maxAvailableRunning = 0;
            if( querySingles(maxAvailableRunning) )
            {
                return updateSuccessful(C0,cycle); // early |Xi| = 0 return
            }

            //------------------------------------------------------------------
            //
            //
            // act according to max available running
            //
            //
            //------------------------------------------------------------------
            switch(maxAvailableRunning)
            {
                case 0:
                    YACK_CHEM_PRINTLN(vpfx << "    [success: all blocked]");
                    return updateSuccessful(C0,cycle);

                case 1:
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        if(!blocked[ei])
                        {
                            active.transfer(Corg,Cl[ei]);
                            goto GLOBAL_STEP;
                        }
                    }
                    YACK_CHEM_PRINTLN(vpfx << "  [failure: corrupted-blocked]");
                    return false;

                default:
                    break;
            }
            assert(maxAvailableRunning>1);

            //------------------------------------------------------------------
            //
            //
            // search for global decrease
            //
            //
            //------------------------------------------------------------------
            bool   foundGlobalDecrease = false;
            double G0                  = Hamiltonian(Corg);
            //if(G0>0) ios::ocstream::echo("g0.dat", "%u %.15g\n", cycle, log10(G0));
            {

                YACK_CHEM_PRINTLN(vpfx << "    [initialized] G0 = " << G0 << " ]");
                const equilibrium *emin = queryLattice(G0);
                if(emin)
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <found global decrease @{" << emin->name << "} >");
                    //----------------------------------------------------------
                    //
                    // found global decrease
                    //
                    //----------------------------------------------------------
                    const double G1 = buildHamiltonian(*emin);
                    if(G1<=0)
                    {
                        YACK_CHEM_PRINTLN(vpfx << "  [success : G1 = 0 ]");
                        return updateSuccessful(C0,cycle);
                    }

                    //----------------------------------------------------------
                    //
                    // recompute sigma for Hamiltonian @Corg
                    //
                    //----------------------------------------------------------
                    if( querySingles(maxAvailableRunning) )
                        return updateSuccessful(C0,cycle);   // early return

                    //----------------------------------------------------------
                    //
                    // set status and update hamiltonian
                    //
                    //----------------------------------------------------------
                    foundGlobalDecrease = true;              // update status
                    G0                  = Hamiltonian(Corg); // update G0 at new Corg/sigma
                    //if(G0>0) ios::ocstream::echo("g0.dat", "%u.5 %.15g\n", cycle, log10(G0));
                }
                else
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <already at global minimum>");
                    //----------------------------------------------------------
                    //
                    // not moved, sigma is OK, G0 OK
                    //
                    //----------------------------------------------------------
                }
            }
            YACK_CHEM_PRINTLN(vpfx << "    [foundGlobalDecrease]=" << yack_boolean(foundGlobalDecrease) );

            //------------------------------------------------------------------
            //
            //
            // search for merged step
            //
            //
            //------------------------------------------------------------------
            initializeOmega0();

            //------------------------------------------------------------------
            //
            // compute jacobian inverse
            //
            //------------------------------------------------------------------
        MERGED_STEP:
            iOmega.assign(Omega0);
            if(!LU->build(iOmega))
            {
                if(foundGlobalDecrease)
                {
                    // try again
                    YACK_CHEM_PRINTLN(vpfx << "  [failure : temporary singular composition]");
                    goto GLOBAL_STEP;
                }
                else
                {
                    // failure
                    YACK_CHEM_PRINTLN(vpfx << "  [failure : permanent singular composition]");
                    return false;
                }
            }

            //------------------------------------------------------------------
            //
            // compute full step
            //
            //------------------------------------------------------------------
            iota::load(xi,Gamma);
            LU->solve(iOmega,xi);


            //------------------------------------------------------------------
            //
            // validate primary constraints
            //
            //------------------------------------------------------------------
            bool   hasRobustPhaseSpace = true;
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
                            --maxAvailableRunning;
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
                    hasRobustPhaseSpace = false;
                    YACK_CHEM_PRINTLN(vpfx << "  [overshoot: #DOF= " << maxAvailableRunning << "]");
                    if(foundGlobalDecrease)
                    {
                        //------------------------------------------------------
                        // try another global decrease to stabilize
                        //------------------------------------------------------
                        YACK_CHEM_PRINTLN(vpfx << "  [overshoot: trying another global step]");
                        goto GLOBAL_STEP;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // exhausted global steps
                        //------------------------------------------------------
                        if(maxAvailableRunning<=0)
                        {
                            // can't move!!
                            YACK_CHEM_PRINTLN(vpfx << "  [overshooting: singular system]");
                            return false;
                        }
                        else
                        {
                            // try a reduced step
                            YACK_CHEM_PRINTLN(vpfx << "  [overshooting: reducing system]");
                            goto MERGED_STEP;
                        }
                    }
                }
            }

            YACK_CHEM_PRINTLN(vpfx << "    [gotAcceptableExtent]@ #" << cycle);
            YACK_CHEM_PRINTLN(vpfx << "    [foundGlobalDecrease]  = " << yack_boolean(foundGlobalDecrease) );
            YACK_CHEM_PRINTLN(vpfx << "    [hasRobustPhaseSpace]  = " << yack_boolean(hasRobustPhaseSpace) );

            if(optimizeFullStep(G0))
            {
                //--------------------------------------------------------------
                //
                // decreased, try net step!
                //
                //--------------------------------------------------------------
                goto GLOBAL_STEP;

            }
            else
            {
                //--------------------------------------------------------------
                //
                // minimization failure
                //
                //--------------------------------------------------------------
                if(foundGlobalDecrease)
                {
                    //----------------------------------------------------------
                    // try another global step
                    //----------------------------------------------------------
                    goto GLOBAL_STEP;
                }
                else
                {
                    if(!hasRobustPhaseSpace)
                    {
                        //------------------------------------------------------
                        // bad composition
                        //------------------------------------------------------
                        YACK_CHEM_PRINTLN(vpfx << "  [failure: weak composition at extremum]");
                        return false;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // at global minimum, and no local, robust decrease
                        //------------------------------------------------------
                        if(validSteadyState())
                        {
                            return updateSuccessful(C0,cycle);
                        }
                        else
                        {
                            return false;
                        }
                    }

                }
            }


        }


        bool reactor:: validSteadyState() throw()
        {

            YACK_CHEM_MARKUP(vpfx, "reactor::validSteadyState");

            bool   isValid = true;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium       &eq  = ***node;
                const size_t             ei  = *eq;
                writable<double>        &psi = Psi[ei];
                writable<double>        &Ci  = Cl[ei];
                const double             Ki  = K[ei];
                const double             xx  = (Xl[ei] = eq.solve1D(Ki,Corg,Ci));
                const double             ax  = fabs(xx);

                if(verbose) singles.pad(std::cerr << vpfx << " scanning @{" << eq.name << "}",eq) << " = " << xx << std::endl;

                if(ax<=0) continue;

                eq.drvs_action(psi,Ki,Ci,Ctry);
                if( sorted::dot(psi,Nu[ei],Ctry) >= 0 )
                {
                    continue; // blocked
                }


                bool underflow = false;

                for(const cnode *scan=eq.head();scan;scan=scan->next)
                {
                    const species &sp = ****scan;
                    const size_t   j  = *sp;
                    const double   c_old = Corg[j];
                    const double   c_new = Ci[j];
                    if(verbose)
                    {
                        lib.pad(std::cerr << vpfx << "\t" << sp.name, sp) << " : ";
                        std::cerr << std::setw(15) << c_old << " --> " << std::setw(15) << c_new;
                    }
                    const bool zero_delta = fabs(c_old-c_new) <= 0;
                    if(zero_delta)
                    {
                        YACK_CHEM_PRINTLN(" | underflow");
                        underflow = true;
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN(" | regular..");
                    }
                }

                if(!underflow)
                {
                    isValid = false;
                }

            }

            YACK_CHEM_PRINTLN(vpfx << "   [[ valid = " << yack_boolean(isValid) << " ]]");
            return isValid;

        }


    }

}

#endif

