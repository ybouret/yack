#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/numeric.hpp"
#include "yack/math/algebra/svd.hpp"
#include <iomanip>
#include <cmath>


namespace yack
{
    using namespace math;

    namespace chemical
    {
        reactor::steady_state  reactor:: updateSuccessful(writable<double> &C0, unsigned cycle) throw()
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
                //std::cerr << "   eps=" << numeric<double>::epsilon << std::endl;
            }
            return steady_success;
        }

        bool reactor:: initializeSearch(writable<double> &C0) throw()
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
                    eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(" <success::1D/>");
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


        reactor::steady_state reactor::steady(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // workspace initialization
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0; ios::ocstream::overwrite("g0.dat");
            if( initializeSearch(C0) )
            {
                return updateSuccessful(C0,cycle); // fastest !
            }

#if 0
            svd<double> SVD(N);
            rmatrix     U(N,N);
            rmatrix     V(N,N);
            rvector     w(N,0);
#endif

        GLOBAL_STEP:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- cycle " << cycle  << " ----------------");

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
                    return steady_failure;

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
            double G0                  = Hamiltonian(Corg);  if(G0>0) ios::ocstream::echo("g0.dat", "%u %.15g\n", cycle, log10(G0));
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
                    if(G0>0) ios::ocstream::echo("g0.dat", "%u.5 %.15g\n", cycle, log10(G0));
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
                    return steady_failure;
                }
            }

            //------------------------------------------------------------------
            //
            // compute full step
            //
            //------------------------------------------------------------------
            iota::load(xi,Gamma);
            LU->solve(iOmega,xi);

            //--------------------------------------------------------------
            //
            // validate primary constraints
            //
            //--------------------------------------------------------------
            bool   hasRobustPhaseSpace = true;
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

                if(overshoot)
                {
                    hasRobustPhaseSpace = false;
                    YACK_CHEM_PRINTLN(vpfx << "  [overshoot: #DOF= " << maxAvailableRunning << "]");
                    if(foundGlobalDecrease)
                    {
                        // try another global decrease to stabilize
                        YACK_CHEM_PRINTLN(vpfx << "  [overshoot: trying another global step]");
                        goto GLOBAL_STEP;
                    }
                    else
                    {
                        // exhausted global steps
                        if(maxAvailableRunning<=0)
                        {
                            // can't move!!
                            YACK_CHEM_PRINTLN(vpfx << "  [overshooting: singular system]");
                            return steady_failure;
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


            exit(1);
            return steady_failure;
        }
        
    }

}


