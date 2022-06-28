#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex/integer.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/optimize.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double plexus:: optimizedCombination(const cluster &cc) throw()
        {
            iota::load(Ctry,Corg);
            for(const vnode *node=cc.head;node;node=node->next)
            {
                const equilibrium      &eq  = **node;
                const size_t            ei  = *eq;
                if(Ok[ei])
                {
                    eq.transfer(Ctry,Cs[ei]); // transfer components only
                }
            }
            return hamiltonian(Ctry);
        }


        double plexus:: searchGlobalDecrease() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "searchGlobalDecrease");

            // initialize with first cluster and gOpt @Cend
            const  cluster *cOpt = com.head;
            double          gOpt = optimizedCombination(*cOpt);
            iota::load(Cend,Ctry);

            // update gOpt and Cend from better cluster
            for(const cluster *cc=cOpt->next;cc;cc=cc->next)
            {
                const double gTmp  = optimizedCombination(*cc);
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    cOpt = cc;
                    iota::load(Cend,Ctry);
                }
            }

            if(verbose)
            {
                std::cerr << vpfx << " => "  << *cOpt << std::endl;
                std::cerr << vpfx << " => @" <<  gOpt << " from " << hamiltonian(Corg) << std::endl;
            }

            // update Corg from best cluster
            iota::load(Corg,Cend);
            return gOpt;
        }


        static inline bool has_winning(const readable<bool> &flags) throw()
        {
            for(size_t i=flags.size();i>0;--i)
            {
                if(flags[i]) return true;
            }
            return false;
        }

        bool plexus:: successful(writable<double> &C0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "successful");
            transfer(C0,Corg);
            if(verbose)
            {
                lib(std::cerr << vpfx  << "C0=",C0,vpfx);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    singles.pad(std::cerr << vpfx << eq.name,eq);
                    std::cerr << " | ma=" << std::setw(15) << eq.mass_action(K[ei],C0);
                    std::cerr << " | Xi=" << std::setw(15) << eq.solve1D(K[ei],C0,Ctry);
                    std::cerr << std::endl;
                }
            }
            return true;
        }


        bool plexus:: solve( writable<double> &C0 ) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "PlexusSolve");
            assert(C0.size()>=M);
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            //------------------------------------------------------------------
            //
            // initializing
            //
            //------------------------------------------------------------------
            switch(N)
            {
                    //----------------------------------------------------------
                case 0:   // no equilibrium
                    //----------------------------------------------------------
                    return true;

                    //----------------------------------------------------------
                case 1: { // special case
                    //----------------------------------------------------------

                    const equilibrium &eq = *** singles.head();
                    (void) eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS-1D/>");
                    return successful(C0);
                }

                    //----------------------------------------------------------
                default: // generic case, prepare consistent workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Cend[j] = Ctry[j] = C0[j];
                        Cstp[j] = 0;
                    }
            }

            unsigned cycle       = 0;
            double   G0          = hamiltonian(Corg);

        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "-------- cycle #" << cycle << " --------");
            //------------------------------------------------------------------
            //
            // check hamiltonian
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "G0 = " << G0);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS G=0 @init/>");
                return successful(C0);
            }

            //------------------------------------------------------------------
            //
            // check singles |Xi|
            //
            //------------------------------------------------------------------
            double AX = computeSinglesExtent(G0); YACK_CHEM_PRINTLN(vpfx << "|Xi| = " << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS |Xi|=0 @init/>");
                return successful(C0);
            }

            //------------------------------------------------------------------
            //
            // updated remaining equations
            //
            //------------------------------------------------------------------
            computeCouplesExtent(G0);

            //------------------------------------------------------------------
            //
            // check status
            //
            //------------------------------------------------------------------
            if( !has_winning(Ok) )
            {
                YACK_CHEM_PRINTLN(vpfx << "  <NO WINNING/>");
                return false;
            }


            //------------------------------------------------------------------
            //
            // search the greatest global decrease among winning
            // equilibria, among clusters
            //
            //------------------------------------------------------------------
            G0 = searchGlobalDecrease();
            lib(std::cerr << vpfx << "Corg=",Corg,vpfx);
            YACK_CHEM_PRINTLN(vpfx << "G0 = " << G0);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS G=0 @move/>");
                return successful(C0);
            }

            //------------------------------------------------------------------
            //
            // get the modified singles extent
            //
            //------------------------------------------------------------------
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       xx = ( Xl[ei] =eq.solve1D(K[ei],Corg,Ctry) );
                Xtry[ei] = fabs(xx);
                if( eq.changed(Corg,xx,Ctry) )
                {
                    YACK_CHEM_PRINTLN(vpfx << eq.name << " is changed for " << xx);
                }
                else
                {
                    YACK_CHEM_PRINTLN(vpfx << eq.name << " underflow for " << xx );
                }
            }
            singles(std::cerr << "|Xi| =",Xtry,"");
            AX = sorted::sum(Xtry,sorted::by_value);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS |Xi| =0 @move/>");
                return successful(C0);
            }



            //------------------------------------------------------------------
            //
            // compute full local metrics
            //
            //------------------------------------------------------------------
            size_t num_running = computeOmega();
            bool   maximum_dof = true;
            YACK_CHEM_PRINTLN(vpfx << "#running=" << num_running);

            switch(num_running)
            {
                case 0:
                    YACK_CHEM_PRINTLN(vpfx << "  <ALL BLOCKED/>");
                    return  successful(C0);

                case 1:
                    assert(N>1);
                    YACK_CHEM_PRINTLN(vpfx << "  <#RUNNING=1/>");
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        if( !blocked[*eq] )
                        {
                            (void) eq.solve1D(K[*eq],Corg,Cend);
                            iota::load(Corg,Cend);
                            goto CYCLE;
                        }
                    }
                    YACK_CHEM_PRINTLN(vpfx << "  <CORRUPTED/>");
                    return false;

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // compute extent, modify local metrics upon overshoot
            //
            //------------------------------------------------------------------
            {
                YACK_CHEM_MARKUP(vpfx, "computeExtent");
                unsigned lap = 0;
            COMPUTE_EXTENT:
                ++lap;
                YACK_CHEM_PRINTLN(vpfx << "-------- extent " << cycle << "." << lap << " --------");
                if(verbose) singles(std::cerr << "blocked=",blocked,"");
                

                //--------------------------------------------------------------
                // solve system
                //--------------------------------------------------------------
                iOmega.assign(Omega0);
                if(!LU->build(iOmega))
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <SINGULAR/>");
                    return false;
                }

                iota::load(xi,Gamma);
                LU->solve(iOmega,xi);

                //--------------------------------------------------------------
                // detect overshoot by primary limits
                //--------------------------------------------------------------
                bool overshoot = false;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    double            &xx = xi[ei];
                    if(verbose) singles.pad(std::cerr << vpfx << "xi_" << eq.name,eq) << " = " << std::setw(15) << xx << ' ';

                    if(blocked[ei])
                    {
                        assert( fabs(xx) <= 0 );
                        YACK_CHEM_PRINT("[blocked]");
                    }
                    else
                    {
                        const limits &lm = eq.primary_limits(Corg,lib.width);
                        if(lm.is_acceptable(xx))
                        {
                            YACK_CHEM_PRINT("[accept] ");
                        }
                        else
                        {
                            YACK_CHEM_PRINT("[REJECT] ");
                            overshoot = true;          // need to recompute
                            suspendEquilibriumAt(ei);  // something bad happened
                            --num_running;             // monitoring...
                        }
                        YACK_CHEM_PRINT(lm);
                    }
                    if(verbose) std::cerr << std::endl;
                }

                //--------------------------------------------------------------
                // retry with modified Omega upon overshoot
                //--------------------------------------------------------------
                if(overshoot)
                {
                    maximum_dof = false;
                    goto COMPUTE_EXTENT;
                }
            }

            //------------------------------------------------------------------
            //
            // compute Cstp
            //
            //------------------------------------------------------------------
            const double G1 = probeCombinedExtents(G0);
            YACK_CHEM_PRINTLN(vpfx << "hamiltonian: " << G0 << " --> " << G1 << " @maximum_dof=" << maximum_dof);
#if 1
            if(G1>=G0)
            {
                if(maximum_dof)
                {
                    successful(C0);
                    exit(1);
                }
            }
#endif
            G0 = G1;


            goto CYCLE;

        }

    }

}
