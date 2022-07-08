
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
        
        double  reactor:: aggregate(writable<double> &C, const group &g) throw()
        {
            assert(g.is_valid());
            assert(g.is_ortho());
            assert(C.size()==Corg.size());
            iota::save(C,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium      &eq = **ep;
                const readable<double> &Ci = Cl[*eq]; assert(eq.other_are_unchanged(Ci,Corg));
                eq.transfer(C,Ci);
            }
            const double res = hamiltonian(C);
            return res;
        }



        
        bool    reactor:: normalize(writable<double> &C0) throw()
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::normalize");
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

            //------------------------------------------------------------------
            //
            //
            // look up
            //
            //
            //------------------------------------------------------------------
            double   G0    = hamiltonian(Corg);
            unsigned cycle = 0;


        CYCLE:
            //------------------------------------------------------------------
            //
            // Global Regularization
            //
            //------------------------------------------------------------------
            ++cycle;
            YACK_CHEM_PRINTLN("---------------- cycle=" << cycle << " ----------------");
            if(verbose) lib(std::cerr << "Corg=",Corg,"");
            YACK_CHEM_PRINTLN(vpfx << "  G0 = " << G0);

            if(G0<=0) {
                YACK_CHEM_PRINTLN(" <success:: null hamiltonian @init/>");
                return returnSuccessful(C0,cycle);
            }


            bool globallyDecreased = true; // check if global dercreasing
            {
                double             Xmax = 0;
                double             Gmin = G0;
                const equilibrium *emax = NULL;
                const equilibrium *emin = NULL;

                //--------------------------------------------------------------
                //
                // scan all equilibria
                //
                //--------------------------------------------------------------
                for(const enode *node=lattice.head();node;node=node->next)
                {
                    //----------------------------------------------------------
                    // compute equilibrium status
                    //----------------------------------------------------------
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const double       xx = (Xl[ei] = eq.solve1D(Kl[ei], Corg, Cend)); assert( eq.other_are_unchanged(Corg,Cend) );
                    const double       ax = fabs( xx );
                    if(ax>Xmax) {
                        Xmax =  ax;
                        emax = &eq;
                    }

                    //----------------------------------------------------------
                    // check for regularisation
                    //----------------------------------------------------------
                    triplet<double> u = { 0,  -1, 1    };
                    triplet<double> g = { G0, -1, hamiltonian(Cend) };
                    optimize::run_for(*this,u,g,optimize::inside);
                    iota::load(Cl[ei],Ctry); assert( eq.other_are_unchanged(Cl[ei],Corg) );

                    lattice.pad(std::cerr << " @{" << eq.name << "}",eq) << " = " << std::setw(15) << xx;
                    std::cerr << " | G = " << std::setw(15) << g.b << " @u=" <<  std::setw(15) << u.b;

                    //----------------------------------------------------------
                    // update best global possibility
                    //----------------------------------------------------------
                    if(g.b<Gmin)
                    {
                        Gmin = g.b;
                        emin = &eq;
                    }
                    std::cerr << std::endl;
                }

                //--------------------------------------------------------------
                //
                // check |Xi| status
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN("|Xi|  = " << std::setw(15) << Xmax << " @{" << (emax? emax->name() : "nil") << "}" );
                if(Xmax<=0) {
                    assert(NULL==emax);
                    YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @init/>");
                    return returnSuccessful(C0,cycle);
                }
                assert(NULL!=emax);

                //--------------------------------------------------------------
                //
                // check global decrease status
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN(" Gmin = " <<  std::setw(15) << Gmin << " @{" << (emin? emin->name() : "NIL") << "}" );
                if(!emin) {
                    globallyDecreased = false;
                }
                else {
                    //----------------------------------------------------------
                    // optimize composition with secondary decrease:
                    // initialize first group containing best equilibrium
                    //----------------------------------------------------------
                    const equilibrium &eq   = *emin;
                    const group       *gOpt = look_up->find_first( eq ); assert(gOpt);
                    tableau           &Copt = Cend;
                    double             hOpt = aggregate(Copt,*gOpt);
                    if(verbose) std::cerr << "(#) G = " << std::setw(15) << hOpt  << " @" << *gOpt << std::endl;

                    //----------------------------------------------------------
                    //
                    // look up in other groups containing equilibirium
                    //
                    //----------------------------------------------------------
                    for(const group   *gTmp = gOpt->next;gTmp;gTmp=gTmp->next)
                    {
                        if(!gTmp->contains(eq)) continue;
                        const double hTmp = aggregate(Ctry,*gTmp);
                        const bool   good = (hTmp<hOpt);
                        if(verbose) std::cerr << (good?"(+)":"(-)") << " G = " << std::setw(15) << hTmp  << " @" << *gTmp << std::endl;
                        if(good)
                        {
                            gOpt = gTmp;
                            hOpt = hTmp;
                            active.transfer(Copt,Ctry);
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // update current status
                    //
                    //----------------------------------------------------------
                    if(verbose) std::cerr << "Gopt  = " << std::setw(15) << hOpt << " @" << *gOpt << " / G0=" << G0 << std::endl;
                    G0 = hOpt;
                    active.transfer(Corg,Copt);


                    //----------------------------------------------------------
                    // check current hamiltonian
                    //----------------------------------------------------------
                    if(G0<=0)
                    {
                        YACK_CHEM_PRINTLN(" <success:: null hamiltonian @move/>");
                        return returnSuccessful(C0,cycle);
                    }

                    //----------------------------------------------------------
                    // recomputing Xmax and Xi for singles
                    //----------------------------------------------------------
                    Xmax = 0;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        const double       xx = (Xend[ei] = eq.solve1D(Kl[ei], Corg, Cl[ei]));
                        const double       ax = fabs( xx );
                        if(ax>Xmax)
                        {
                            Xmax = ax;
                        }
                    }
                    if(verbose)
                    {
                        lib(std::cerr << "Copt=",Corg,"");
                        singles(std::cerr << vpfx << "Xi_singles=",Xend,vpfx);
                    }
                    if(Xmax<=0)
                    {
                        YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @move/>");
                        return returnSuccessful(C0,cycle);
                    }
                }

                YACK_CHEM_PRINTLN(vpfx << "[globallyDecreased=" << yack_boolean(globallyDecreased) << "]");
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
                    YACK_CHEM_PRINTLN(" <singular local system>");
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
    }


}
