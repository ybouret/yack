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


        reactor::steady_state reactor::steady(writable<double> &C0) throw()
        {
            // workspace initialization
            unsigned cycle = 0; ios::ocstream::overwrite("g0.dat");
            if( initializeSearch(C0) )
            {
                return updateSuccessful(C0,cycle); // fastest !
            }


        GLOBAL_STEP:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- cycle " << cycle  << " ----------------");

            // compute all single extents and sigma for hamiltonian
            size_t maxAvailableRunning = 0;
            if( querySingles(maxAvailableRunning) )
            {
                // early return
                return updateSuccessful(C0,cycle); // early |Xi| = 0 return
            }

            // act according to max available running
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
            



            exit(1);
            return steady_failure;
        }
        
    }

}


