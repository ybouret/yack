

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

        

        void   reactor:: zapEquilibriumAt(const size_t ei) throw()
        {
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            sigma[ei]   = 0;
            Gamma[ei]   = 0;
            Omi.ld(0); Omi[ei] = 1.0;
            NuA[ei].ld(0);
        }




        
        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::solve");
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);

            ios::ocstream::overwrite("g0.dat");

            //------------------------------------------------------------------
            //
            //
            // checking cases
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0;
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
                } return returnSuccessful(C0,cycle);

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



        MAJOR_STEP:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- cycle " << cycle  << " ----------------");
            size_t maxMinorDimensions = 0;
            //------------------------------------------------------------------
            //
            // @Corg:
            // - query singles for |Xi| and sigma
            // - count running equilibria
            //
            //------------------------------------------------------------------
            {
                if( querySingles(maxMinorDimensions) )
                {
                    return returnSuccessful(C0,cycle); // early return |Xi| = 0
                }

                switch(maxMinorDimensions)
                {
                    case 0:
                        YACK_CHEM_PRINTLN(vpfx << "    [success: all blocked]");
                        return returnSuccessful(C0,cycle);

                    case 1:
                        for(const enode *node=singles.head();node;node=node->next)
                        {
                            const equilibrium &eq = ***node;
                            const size_t       ei = *eq;
                            if(!blocked[ei])
                            {
                                active.transfer(Corg,Cl[ei]);
                                goto MAJOR_STEP;
                            }
                        }
                        YACK_CHEM_PRINTLN(vpfx << "  [failure: corrupted-blocked]");
                        return false;

                    default:
                        break;
                }
                assert(maxMinorDimensions>1);
            }


            //------------------------------------------------------------------
            //
            //
            // @Corg, |Xi|>0, so I assume G0>0
            //
            //
            //------------------------------------------------------------------
            bool   foundMajorDecrease = false;
            double G0                 = Hamiltonian(Corg);  if(G0>0) ios::ocstream::echo("g0.dat", "%u %.15g\n", cycle, log10(G0));
            {

                YACK_CHEM_PRINTLN(vpfx << "    [initialized] G0 = " << G0 << " ]");
                const equilibrium *emin = queryLattice(G0);
                if(emin)
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <found global decrease @{" << emin->name << "} >");
                    //----------------------------------------------------------
                    //
                    // found major decrease
                    //
                    //----------------------------------------------------------
                    const double G1 = buildHamiltonian(*emin);
                    if(G1<=0)
                    {
                        YACK_CHEM_PRINTLN(vpfx << "  [success : G1 = 0 ]");
                        return returnSuccessful(C0,cycle);
                    }

                    //----------------------------------------------------------
                    //
                    // recompute sigma for local step
                    //
                    //----------------------------------------------------------
                    if( querySingles(maxMinorDimensions) )
                    {
                        return returnSuccessful(C0,cycle); // early return
                    }
                    foundMajorDecrease = true;              // update status
                    G0                 = Hamiltonian(Corg); // update G0 at new Corg/sigma
                    if(G0>0) ios::ocstream::echo("g0.dat", "%u.5 %.15g\n", cycle, log10(G0));
                }
                else
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <found global minimum>");
                    //----------------------------------------------------------
                    //
                    // not moved, sigma is OK, G0 OK
                    //
                    //----------------------------------------------------------
                }
            }
            YACK_CHEM_PRINTLN(vpfx << "    [foundMajorDecrease]=" << yack_boolean(foundMajorDecrease) );


            //------------------------------------------------------------------
            //
            //
            // from Corg/sigma: build Omega0
            //
            //
            //------------------------------------------------------------------
            updateOmega0();
            bool   hasMinorRobustness = true; // all non-blocked eqs are active
            size_t minor              = 0;

        MINOR_STEP:
            ++minor;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- minor " << cycle  << "." << minor << "----------------");

            iOmega.assign(Omega0);
            if(!LU->build(iOmega))
            {
                if(foundMajorDecrease)
                {
                    YACK_CHEM_PRINTLN(vpfx << "  [extent : retry major step]");
                    goto MAJOR_STEP;
                }
                else
                {
                    // can't go further
                    YACK_CHEM_PRINTLN(vpfx << "  [extent : singular system]");
                    return false;
                }
            }

            //------------------------------------------------------------------
            //
            // compute xi
            //
            //------------------------------------------------------------------
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
                            YACK_CHEM_PRINT("[REJECT]");
                            zapEquilibriumAt(ei);
                            --maxMinorDimensions;
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
                    hasMinorRobustness = false;
                    YACK_CHEM_PRINTLN(vpfx << "  [overshoot: #DOF= " << maxMinorDimensions << "]");
                    if(foundMajorDecrease)
                    {
                        // try another global decrease to stabilize
                        YACK_CHEM_PRINTLN(vpfx << "  [overshoot: trying another global step]");
                        goto MAJOR_STEP;
                    }
                    else
                    {
                        // exhausted major steps
                        if(maxMinorDimensions<=0)
                        {
                            // can't move!!
                            YACK_CHEM_PRINTLN(vpfx << "  [overshooting: singular system]");
                            return false;
                        }
                        else
                        {
                            // try a reduced step
                            YACK_CHEM_PRINTLN(vpfx << "  [overshooting: reducing system]");
                            exit(1);
                            goto MINOR_STEP;
                        }
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // found an acceptable extent!
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "    [acceptable extent!]");
            YACK_CHEM_PRINTLN(vpfx << "    [foundMajorDecrease]=" << yack_boolean(foundMajorDecrease) );
            YACK_CHEM_PRINTLN(vpfx << "    [hasMinorRobustness]=" << yack_boolean(hasMinorRobustness) );



            //------------------------------------------------------------------
            //
            // minimisation
            //
            //------------------------------------------------------------------
            if( optimizeFullStep(G0) )
            {

                YACK_CHEM_PRINTLN(vpfx << "    [optimized minor step]");
                goto MAJOR_STEP;
            }
            else
            {
                YACK_CHEM_PRINTLN(vpfx << "    [not-decreasing minor step]");
                if(foundMajorDecrease)
                {
                    // try again
                    goto MAJOR_STEP;
                }
                else
                {
                    // sigma/omega are computed
                    if(hasMinorRobustness)
                    {
                        YACK_CHEM_PRINTLN(vpfx << "  [NEED TO CHECK]");
                        for(const enode *node=singles.head();node;node=node->next)
                        {
                            const equilibrium &eq = ***node;
                            const size_t       ei = *eq;
                            singles.pad(std::cerr << "@{" << eq.name << "}",eq) << " :";
                            std::cerr << "xi=" << std::setw(15) << Xl[ei];
                            if(eq.found_underflow_for(Xl[ei],Corg))
                            {
                                std::cerr << " [ok]";
                            }
                            else
                            {
                                std::cerr << " [NO]";
                            }
                            std::cerr << std::endl;
                        }

                        singles(std::cerr << "Omega=",Omega0,"");
                        singles(std::cerr << "Sigma=",sigma,"");

                        (void) returnSuccessful(C0,cycle);

                        exit(1);
                    }
                    else
                    {
                        // !bad!
                        YACK_CHEM_PRINTLN(vpfx << "  [singular extremum]");
                        return false;
                    }
                }

            }



        }


    }

}


