
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
        double reactor:: hamiltonian(const readable<double> &C) throw()
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtry,sorted::by_value);
        }
        
        double reactor:: operator()(const double u) throw()
        {
            const double v = 1.0 - u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = v * Corg[j] + u * Cend[j];
            }
            return hamiltonian(Ctry);
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
                } return onSuccess(C0);
                    
                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }
            
            double   G0    = hamiltonian(Corg);
            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: G0 = 0 @init/>");
                return onSuccess(C0);
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
                return onSuccess(C0);
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
                return onSuccess(C0);
            }

            AX = upgradeSinglesXi();
            YACK_CHEM_PRINTLN(vpfx << "|Xi| =" << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @move/>");
                return onSuccess(C0);
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
        
        
        double reactor:: minimizeFullStep(const double G0) throw()
        {
            
            //------------------------------------------------------------------
            //
            // Cstp = NuA' * xi and deduce limiting ratio
            //
            //------------------------------------------------------------------
            ratio.free();
            {
                const size_t n = N;
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    for(size_t i=n;i>0;--i)
                    {
                        Xtry[i] = xi[i] * NuA[i][j];
                    }
                    const double d = (Cstp[j] = sorted::sum(Xtry,sorted::by_abs_value));
                    if(d<0)
                    {
                        const double c = Corg[j]; assert(c>=0);
                        ratio << (c/-d);
                    }
                }
            }
            
            double expand = 2.0;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                std::cerr << vpfx << "ratio=" << ratio << std::endl;
                expand = min_of(expand,0.99*ratio.front());
            }
            
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Cend[j] = max_of(Corg[j]+Cstp[j]*expand,0.0);
            }
            
            if(verbose)
            {
                lib(std::cerr << vpfx << "Cend=",Cend,vpfx);
            }
            
            triplet<double> u = { 0,  -1, 1 };
            triplet<double> g = { G0, -1, hamiltonian(Cend) };
            
            if(true)
            {
                ios::ocstream fp("hamiltonian.dat");
                const size_t  np = 1000;
                for(size_t i=0;i<=np;++i)
                {
                    const double uu = i/double(np);
                    fp("%.15g %.15g\n",uu, (*this)(uu));
                }
            }
            
            optimize::run_for(*this, u, g, optimize::inside);
            std::cerr << "G: " << G0 << " --> " << g.b << std::endl;
            active.transfer(Corg,Ctry);
            return g.b;
        }
        
        void   reactor:: zapEquilibriumAt(const size_t ei) throw()
        {
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            Gamma[ei]   = 0;
            Omi.ld(0); Omi[ei] = 1.0;
            NuA[ei].ld(0);
        }
        
        size_t reactor:: initializeOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::initializeLocalStep");
            
            size_t num_running = N;
            NuA.assign(Nu);
            
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega0[ei];
                double            &gam = Gamma[ei];
                
                gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                const double den = - sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    blocked[ei] = true;
                    Omi.ld(0); Omi[ei] = 1.0;
                    gam=0;
                    NuA[ei].ld(0);
                    --num_running;
                }
                else
                {
                    blocked[ei] = false;
                    Omi[ei]     = den;
                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }
                    
                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }
                    
                    if( fabs(Xend[ei]) <= 0 ) gam = 0;
                }
                
            }
            if(verbose)
            {
                singles(std::cerr<<vpfx<<"blocked=",blocked,vpfx);
                singles(std::cerr<<vpfx<<"Omega=",Omega0,vpfx);
                singles(std::cerr<<vpfx<<"Gamma=",Gamma,vpfx);
            }
            
            return num_running;
            
        }
        
        
        
        bool reactor:: happensUnderflow() throw()
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(eq.changed(Corg, Xend[ei], Ctry)) return false;
                
            }
            
            // nothing has changed!
            return true;
        }
        
        double reactor:: mixedHamiltonian(const group &g, writable<double> &C) throw()
        {
            assert(g.is_valid());
            assert(g.size>0);
            assert(g.is_ortho());

            iota::save(C,Corg); // load initial value
            for(const gnode *pp=g.head;pp;pp=pp->next)
            {
                const equilibrium &eq = **pp;
                eq.transfer(C,Cl[*eq]); // transfer only changed components
            }
            return hamiltonian(C);
        }

        void   reactor:: betterMassAction(double &G0) throw()
        {
            assert( look_up.is_valid() );
            const group *gOpt = look_up->head;                 // initialize
            double       hOpt = mixedHamiltonian(*gOpt,Cend); // initialize @Cend

            std::cerr << " + " << std::setw(15) << hOpt << " @" << *gOpt << std::endl;


            for(const group *g=gOpt->next;g;g=g->next)
            {
                const double h = mixedHamiltonian(*g,Ctry);
                if(h<=hOpt)
                {
                    hOpt = h;
                    gOpt = g;
                    active.transfer(Cend,Ctry);
                    std::cerr << " + " << std::setw(15) << h << " @" << *g << std::endl;
                }
                else
                {
                    std::cerr << " - " << std::setw(15) << h << " @" << *g << std::endl;
                }
            }

            std::cerr << " * " << std::setw(15) << hOpt << " @" << *gOpt << std::endl;
            assert( fabs( hamiltonian(Cend) - hOpt) <=0 );
            G0 = hOpt;
        }

        
        double reactor:: upgradeSinglesXi() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::singlesXi::update");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Cl[ei];
                Xend[ei] = eq.solve1D(K[ei],Corg,Ci) ;
            }
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xend,vpfx);
            return sorted::sum(Xtry,sorted::by_value);
        }



        double   reactor:: studyEquilibrium(const equilibrium &eq, double &G0) throw()
        {
            const size_t       ei = *eq;                           // get index
            const double       xx = eq.solve1D(Kl[ei],Corg,Cend);  // full step
            double             G1 = hamiltonian(Cend);             // full step hamiltonian
            triplet<double>    u  = { 0, -1, 1 };                  // prepare look up [0..1] * xx
            triplet<double>    g  = { G0, -1, G1 };                // values

            optimize::run_for(*this,u,g,optimize::inside);         // optimization step


            iota::save(Cl[ei],Ctry);
            lattice.pad(std::cerr << eq.name,eq) <<  "@xi=" << std::setw(15) << xx << " G: " << G0 << " --> " << G1 << " -> " << g.b << "@u=" << u.b << std::endl;

            assert(g.b<=G0);
            G0 = g.b;
            return xx;

        }

        double reactor:: computeSinglesXi(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::singlesXi");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                double             G1 = G0;

                Xtry[ei] = fabs(  Xl[ei] = Xend[ei] = studyEquilibrium(eq,G1) );
                //Ok[ei]   = (G1 <= G0);
            }
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xend,vpfx);
            return sorted::sum(Xtry,sorted::by_value);
        }
        
        void reactor:: computeCouplesXi(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::couplesXi");
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                double             G1 = G0;
                Xl[ei] = studyEquilibrium(eq,G1);
                //Ok[ei] = (G1 <= G0);
            }
            if(verbose)
            {
                lattice(std::cerr << vpfx << "Xi=",Xl,vpfx);
                //lattice(std::cerr << vpfx << "Ok=",Ok,vpfx);
            }
            
        }
        
        bool reactor:: onSuccess(writable<double> &C0)
        {
            active.transfer(C0,Corg);
            if(verbose)
            {
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
