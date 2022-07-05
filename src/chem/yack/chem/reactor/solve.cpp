#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include <iomanip>
#include <cmath>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {

        
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
            if(verbose) {
                lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);
            }
            
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






            return false;

            double   G0    = hamiltonian(Corg);
            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("  ---------------- cycle #" << cycle << " ----------------");
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: G0 = 0 @init/>");
                return returnSuccessful(C0,cycle);
            }
            
            //------------------------------------------------------------------
            //
            //
            // Pass 1/2: compute singles and |Xi|
            //
            //
            //------------------------------------------------------------------
            double AX = computeSinglesXi(G0);
            YACK_CHEM_PRINTLN(vpfx << "|Xi| =" << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 />");
                return returnSuccessful(C0,cycle);
            }
            
            //------------------------------------------------------------------
            //
            //
            // Pass 1/2: compute remaining couples
            //
            //
            //------------------------------------------------------------------
            computeCouplesXi(G0);
            
            
            //------------------------------------------------------------------
            //
            //
            // try a global hamiltonian decrease
            //
            //
            //------------------------------------------------------------------
            betterMassAction(G0);

            YACK_CHEM_PRINTLN("G0=" << G0);
            active.transfer(Corg,Cend);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: G0 = 0 @move/>");
                return returnSuccessful(C0,cycle);
            }

            AX = upgradeSinglesXi();
            YACK_CHEM_PRINTLN(vpfx << "|Xi| =" << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @move/>");
                return returnSuccessful(C0,cycle);
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
                size_t    num_running = initializeOmega0();
                bool      maximum_dof = true;
                unsigned  inner       = 0;
                
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
                        YACK_CHEM_PRINTLN(" <failure::all-blocked>");
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
                                G0 = hamiltonian(Corg);
                                goto CYCLE;
                            }
                        }
                        YACK_CHEM_PRINTLN(" <failure::corrupted-1D>");
                        break;
                        
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
                    YACK_CHEM_PRINTLN(" <singular system>");
                    goto CYCLE; // try another global step
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
                            YACK_CHEM_PRINT( " " << std::setw(15) << xx << "|" << lm) ;
                        }
                        if(verbose) std::cerr << std::endl;
                    }
                    
                    if(overshoot)
                    {
                        maximum_dof = false;
                        goto COMPUTE_EXTENT;
                    }
                }
                YACK_CHEM_PRINTLN("maximum_dof=" << yack_boolean(maximum_dof) );
            }
            
            minimizeFullStep(G0);
            
            
            if(cycle>=10)
                exit(1);
            
            
            goto CYCLE;
            
            
            
            
            
            return false;
        }
        
        
      
      
        
    
        
       
        
        bool reactor:: returnSuccessful(writable<double> &C0, const unsigned int cycle)
        {
            active.transfer(C0,Corg);
            if(verbose)
            {
                if(cycle>0)
                {
                     YACK_CHEM_PRINTLN("  ---------------- leave #" << cycle << " ----------------");
                }
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    singles.pad(std::cerr << eq.name,eq);
                    std::cerr << " | ma=" << std::setw(15) << eq.mass_action(K[*eq],C0);
                    std::cerr << " | xi=" << std::setw(15) << eq.solve1D(K[*eq],C0,Ctry);
                    std::cerr << std::endl;
                }
            }
            return true;
        }
        
        
    }
    
}
