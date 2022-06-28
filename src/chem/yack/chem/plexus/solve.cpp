#include "yack/chem/plexus.hpp"
#include "yack/chem/eqs/cross.hpp"
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

        //----------------------------------------------------------------------
        //
        // hamiltonian of the system
        //
        //----------------------------------------------------------------------
        double plexus:: hamiltonian(const readable<double> &C)
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtry,sorted::by_value)/N;
        }

        //----------------------------------------------------------------------
        //
        // hamiltonian( (1-u) * Corg + u * Cend )
        //
        //----------------------------------------------------------------------
        double plexus:: operator()(const double u)
        {
            const double v = 1.0-u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = v * Corg[j] + u * Cend[j];
            }
            return hamiltonian(Ctry);
        }

        double plexus:: computePartialExtent(const double G0, const equilibrium &eq) throw()
        {
            //------------------------------------------------------------------
            //
            // compute extent and solution in Cend
            //
            //------------------------------------------------------------------
            const size_t      ei  = *eq;
            const double      ax  = fabs( Xl[ei] = eq.solve1D(Kl[ei],Corg,Cend) );
            writable<double> &Ceq = Cs[ei];

            if(ax>0)
            {
                //--------------------------------------------------------------
                //
                // out of equilibrium
                //
                //--------------------------------------------------------------
                const double G1 = hamiltonian(Cend);
                if(G1<G0)
                {
                    //----------------------------------------------------------
                    // keep new extent @Cend
                    //----------------------------------------------------------
                    iota::load(Ceq,Cend);
                    if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : " << G0 << " --> " << G1 << std::endl;
                }
                else
                {
                    //----------------------------------------------------------
                    // compensated by other equilibria => stalled @Corg
                    //----------------------------------------------------------
                    iota::load(Ceq,Corg);
                    if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : stalled" << std::endl;
                }
            }
            else
            {
                assert(ax<=0);
                //--------------------------------------------------------------
                // numerical equilibrium, |Xi|<=0, keep Corg
                //--------------------------------------------------------------
                if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : ready " << std::endl;
                iota::load(Ceq,Corg);
            }

            std::cerr << "C{" << eq.name << "} = " << Ceq << " / ma=" << eq.mass_action(Kl[ei],Ceq) << std::endl;
            return ax;
        }

        double plexus:: computeSinglesExtent(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeSinglesExtent");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = computePartialExtent(G0,***node); assert(Xtry[ei]>=0);
            }
            return sorted::sum(Xtry,sorted::by_value);
        }

        void plexus:: computeCouplesExtent(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeCouplesExtent");
            for(const enode *node=couples.head();node;node=node->next)
            {
                (void) computePartialExtent(G0,***node);
            }
        }

        

        double plexus:: optimizedCombination(const cluster &cc) throw()
        {
            iota::load(Ctry,Corg);
            for(const vnode *node=cc.head;node;node=node->next)
            {
                const equilibrium      &eq  = **node;
                const readable<double> &Ceq = Cs[*eq];
                eq.transfer(Ctry,Ceq); // transfer components only
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
            std::cerr << " GG = " << hamiltonian(Corg) << std::endl;
            std::cerr << " C1 = " << Corg << std::endl;

            // restabilizing with chosen cluster
            for(const vnode *node=cOpt->head;node;node=node->next)
            {
                const equilibrium &eq = **node;
                const size_t       ei = *eq;
                (void) eq.solve1D(Kl[ei],Corg,Ctry);
                if(ei<=N)
                {
                    Xl[ei]      = 0;
                }
                YACK_CHEM_PRINTLN(vpfx << " (^) " << eq.name);
                iota::load(Corg,Ctry);
            }
            std::cerr << " C2 = " << Corg << std::endl;

            return hamiltonian(Corg);

        }


        size_t plexus:: computeOmega() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeOmega");
            size_t num_running = N;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                writable<double>    &psi = Psi[ei];
                writable<double>    &Omi = Omega0[ei];
                writable<int>       &nuA = NuA[ei];
                const readable<int> &nui = Nu[ei];
                double              &Gam = Gamma[ei];

                Gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                const double d = sorted::dot(psi,nui,Ctry);
                if(d>=0)
                {
                    --num_running;
                    Omi.ld(0);
                    nuA.ld(0);
                    Omi[ei]     = 1.0;
                    Gam         = 0.0;
                    blocked[ei] = true;
                }
                else
                {
                    if( fabs(Xl[ei]) <= 0) Gam = 0.0; // regularize a little
                    iota::load(nuA,nui);
                    blocked[ei] = false;
                    Omi[ei] = -d;
                    for(const enode *scan=node->prev;scan;scan=scan->prev)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                    for(const enode *scan=node->next;scan;scan=scan->next)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                }
            }

            singles(std::cerr << "Omega=",Omega0,"");
            return num_running;
        }

        void   plexus:: suspendEquilibriumAt(const size_t ei) throw()
        {
            assert(!blocked[ei]);
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            NuA[ei].ld(0); // set inactive
            Xl[ei]    = 0; // block Xi
            Gamma[ei] = 0; // block Gamma
            Omi.ld(0);     // set inactive Omega
            Omi[ei] = 1.0; // regularize
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
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS-1D/>");
                } return true;

                    //----------------------------------------------------------
                default: // generic case, prepare consistent workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Cend[j] = Ctry[j] = C0[j];
                        Cstp[j] = 0;
                    }
            }

            unsigned cycle=0;

        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "-------- cycle #" << cycle << " --------");
            //------------------------------------------------------------------
            //
            // check status of singles |Xi|
            //
            //------------------------------------------------------------------
            double G0 = hamiltonian(Corg); YACK_CHEM_PRINTLN(vpfx << "G0 = " << G0);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS G=0 @init/>");
                transfer(C0,Corg);
                return true;
            }

            double AX = computeSinglesExtent(G0); YACK_CHEM_PRINTLN(vpfx << "|Xi| = " << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS |Xi|=0 @init/>");
                transfer(C0,Corg);
                return true;
            }

            //------------------------------------------------------------------
            //
            // updated remaining equations
            //
            //------------------------------------------------------------------
            computeCouplesExtent(G0);


            //------------------------------------------------------------------
            //
            // search the greatest global decrease and post-process
            // global indicators
            //
            //------------------------------------------------------------------
            G0 = searchGlobalDecrease();
            lib(std::cerr << vpfx << "Corg=",Corg,vpfx);
            YACK_CHEM_PRINTLN(vpfx << "G0 = " << G0);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS G=0 @move/>");
                transfer(C0,Corg);
                return true;
            }
            



            exit(1);
            goto CYCLE;

#if 0

            //------------------------------------------------------------------
            //
            // check status of |Xi| while computing all extents
            //
            //------------------------------------------------------------------
            unsigned cycle = 0;
            bool     first = true;
            double   minAX = -1;

        CYCLE:
            ++cycle;
            const double sumAbsXi = computeLatticeExtent();
            YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);


            if(sumAbsXi<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS/>");
                transfer(C0,Corg);
                return true;
            }

            if(first)
            {
                first = false;
                minAX = sumAbsXi;
            }
            else
            {
                assert(minAX>0);
                YACK_CHEM_PRINTLN(vpfx << "|Xi| : from " << minAX << " to " << sumAbsXi);
                minAX = sumAbsXi;
            }


            //------------------------------------------------------------------
            //
            // then choose the most promising cluster
            //
            //------------------------------------------------------------------
            searchGlobalDecrease();
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);

            //------------------------------------------------------------------
            //
            // compute full local metrics
            //
            //------------------------------------------------------------------
            bool   ready       = true;
            size_t num_running = computeOmega();

            YACK_CHEM_PRINTLN(vpfx << "#running=" << num_running);

            switch(num_running)
            {
                case 0:
                    YACK_CHEM_PRINTLN(vpfx << "  <BLOCKED/>");
                    transfer(C0,Corg);
                    return true;

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
                singles(std::cerr << "blocked=",blocked,"");

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
                    ready = false;
                    goto COMPUTE_EXTENT;
                }

            }
            
            //------------------------------------------------------------------
            //
            // compute Cstp
            //
            //------------------------------------------------------------------
            rvector ratio(M,as_capacity);
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node; assert(Corg[j]>=0);
                for(size_t i=N;i>0;--i)
                {
                    Xtry[i] = NuA[i][j] * xi[i];
                }
                const double d = (Cstp[j] = sorted::sum(Xtry,sorted::by_abs_value));
                if(d<0)
                {
                    const double c = Corg[j];
                    ratio << (c/-d);
                }
            }

            double expand = 2.0;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                //ratio.keep_only_front();
                YACK_CHEM_PRINTLN("ratio=" << ratio);
                expand = 0.99 * ratio.front();
            }
            else
            {
                YACK_CHEM_PRINTLN(" [unlimited]");
            }

            {
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node; assert(Corg[j]>=0);
                    Cend[j] = max_of(0.0,Corg[j]+expand*Cstp[j]);
                }

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

                const double    G0 = hamiltonian(Corg);
                triplet<double> u  = { 0, -1, 1 };
                triplet<double> g  = { G0, -1, hamiltonian(Cend) };
                optimize::run_for(*this,u,g,optimize::inside);

                iota::load(Corg,Ctry);
                std::cerr << "reached " << g.b << " from " << G0 << std::endl;

            }

            if(cycle>=10)
            {
                exit(1);
            }
            goto CYCLE;

            return false;
#endif
        }

    }

}
