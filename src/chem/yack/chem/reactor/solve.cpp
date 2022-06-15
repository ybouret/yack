
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/opt/optimize.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        void   reactor:: retractEquilibriumAt(const size_t ei)  throw()
        {
            coerce(NuTA).ld_col(ei,0);
            Omega0.ld_col(ei,0);
            Omega0.ld_row(ei,0);
            Omega0[ei][ei] = 1.0;
            Gamma[ei]      = 0;
            blocked[ei]    = true;
            Psi[ei].ld(0);
        }

        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_MARKUP("//  ", "plexus::solve");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);
            

            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("//      [empty]");
                    return true;

#if 1
                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("//      [1D success]");
                    return true;
#endif

                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = Cend[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // initializing search
            //
            //
            //------------------------------------------------------------------
            bool   ready = true;
            bool   first = true;
            double minXi = 0;
            size_t cycle = 0;
            double G0    = meanGammaSquared(Corg);

        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("//     ================ cycle#" << cycle << " ================");
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);

            //------------------------------------------------------------------
            //
            // starting point for this cycle: G0 and Corg are computed
            //
            //------------------------------------------------------------------
            {
                YACK_CHEM_MARKUP("//    ", "plexus::global_step");


                //--------------------------------------------------------------
                //
                // testing singles and |Xi| convergence
                //
                //--------------------------------------------------------------
                double absXi = 0;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;     assert(ei>0); assert(ei<=N);
                    const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                    if(ax>0)
                    {
                        absXi += ax;
                        Gtot[ei]  = optimizeDecreaseFrom(G0);
                        transfer(Ctot[ei],Ctry);
                    }
                    else
                    {
                        Gtot[ei] = G0;
                        transfer(Ctot[ei],Corg);
                    }
                    if(verbose)
                    {
                        couples.pad(std::cerr << vpfx << eq.name,eq) << " : Xi=" << std::setw(15) << Xtot[ei] << " : " << "Gopt=" << std::setw(15)<< Gtot[ei] << std::endl;
                    }
                }



                if(absXi<=0)
                {
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN("//      [SUCCESS |Xi|=0]");
                    return true;
                }

                if(first)
                {
                    minXi = absXi;
                    first = false;
                }
                else
                {
                    YACK_CHEM_PRINTLN("//      [READY=" << (ready ? "YES" : "NO") << "]");
                    if(ready)
                    {
                        if(absXi>=minXi)
                        {
                            transfer(C0,Corg);
                            YACK_CHEM_PRINTLN("//      [SUCCESS |Xi|@min=" << absXi << "]");
                            eqs(    std::cerr << vpfx << "Xi_single=",Xtot,vpfx);
                            couples(std::cerr << vpfx << "Xi_couple=",Xtot,vpfx);
                            return true;
                        }
                    }
                    minXi = min_of(absXi,minXi);
                }

                //--------------------------------------------------------------
                //
                // testing couples
                //
                //--------------------------------------------------------------
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;     assert(ei>N); assert(ei<=Ntot);
                    const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                    if(ax>0)
                    {
                        Gtot[ei]  = optimizeDecreaseFrom(G0);
                        transfer(Ctot[ei],Ctry);
                        if(verbose)
                        {
                            couples.pad(std::cerr << vpfx << eq.name,eq) << " : Xi=" << std::setw(15) << Xtot[ei] << " : " << "Gopt=" << std::setw(15)<< Gtot[ei] << std::endl;
                        }
                    }
                    else
                    {
                        Gtot[ei] = G0;
                        transfer(Ctot[ei],Corg);
                    }
                }
                YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << absXi);

                //--------------------------------------------------------------
                //
                // select best among singles or couples,
                // which ends the global step
                //
                //--------------------------------------------------------------
#if 0
                const double Gopt = selectDecreasedState();
                if(Gopt>=G0)
                {
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN("//      [SUCCESS |G0|@min=" << G0 << "]");
                    couples(std::cerr << vpfx << "Xi=",Xtot,vpfx);
                    return true;
                }
                G0 = Gopt;
#endif
                G0 = selectDecreasedState();
            }


            //------------------------------------------------------------------
            //
            // compute full metrics
            //
            //------------------------------------------------------------------
            size_t num_running = computeOmegaAndGamma();
            ready              = true;


            //------------------------------------------------------------------
            //
            // evaluate extent
            //
            //------------------------------------------------------------------
            {
                YACK_CHEM_MARKUP("//    ", "plexus::evaluate_extent");
                size_t subCycle = 0;
            EVALUATE_EXTENT:
                ++subCycle;
                YACK_CHEM_PRINTLN("//       ---------------- inner#" << cycle << '.' << subCycle << " ----------------");
                YACK_CHEM_PRINTLN("//       \\_#running = " << num_running << " / " << N );

                //------------------------------------------------------------------
                //
                // specific cases depending on #running
                //
                //------------------------------------------------------------------
                switch(num_running)
                {
                    case 0: // all blocked => spurious success...
                        transfer(C0,Corg);
                        YACK_CHEM_PRINTLN("//      [spurious]");
                        return true;

                    case 1: // solve remaining equilibrium, CYCLE again
                        for(const enode *node=eqs.head();node;node=node->next)
                        {
                            const equilibrium &eq = ***node;
                            const size_t       ei = *eq;
                            if(!blocked[ei])
                            {
                                eq.solve1D(K[ei],Corg,Ctry);
                                transfer(Corg,Ctry);
                                YACK_CHEM_PRINTLN("//      [1D remaining]");
                                break;
                            }
                        }

                        goto CYCLE;

                    default:
                        break;
                }



                //------------------------------------------------------------------
                //
                // compute Newton's step
                //
                //------------------------------------------------------------------
                if(verbose) eqs(std::cerr << vpfx << "Omega=",Omega0,vpfx);

                iOmega.assign(Omega0);
                if(!LU.build(iOmega))
                {
                    YACK_CHEM_PRINTLN("//    [SINGULAR]");
                    return false;
                }

                tao::v1::set(xi,Gamma);
                LU.solve(iOmega,xi);


                //------------------------------------------------------------------
                //
                // check xi against primary limits
                //
                //------------------------------------------------------------------
                bool modified = false;
                for(const enode *node = eqs.head(); node; node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const double       xx = xi[ei];
                    if(verbose) eqs.pad(std::cerr << " (*) " << eq.name,eq) << " : ";

                    if(blocked[ei])
                    {
                        YACK_CHEM_PRINT("[blocked]");
                        assert( fabs(xi[ei]) <= 0 );
                    }
                    else
                    {
                        const limits      &lm = eq.primary_limits(Corg,lib.width);
                        if(verbose) std::cerr << lm;
                        if( lm.should_reduce(xx) )
                        {
                            YACK_CHEM_PRINT(" [reject " << xx << "]");
                            modified = true;
                            --num_running;
                            retractEquilibriumAt(ei);
                        }
                        else
                        {
                            YACK_CHEM_PRINT(" [accept " << xx << "]");
                        }

                    }

                    if(verbose) std::cerr << std::endl;
                }
                if(modified)
                {
                    ready = false;
                    goto EVALUATE_EXTENT;
                }
                
            }


            //------------------------------------------------------------------
            //
            // compute dC and limitations upon negative increment
            //
            //------------------------------------------------------------------
            {
                YACK_CHEM_MARKUP("//    ", "plexus::compute_deltaC");
                rstack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    const double d = (dC[j] = sorted::dot(xi,NuTA[j],Xtmp));
                    const double c = Corg[j]; assert(c>=0);
                    if(d<0)
                    {
                        rstack << c/(-d);
                    }
                }

                //--------------------------------------------------------------
                //
                // compute search extension
                //
                //--------------------------------------------------------------
                double expand = 2.0;
                if(rstack.size())
                {
                    hsort(rstack,comparison::increasing<double>);
                    expand = min_of(expand,0.99*rstack.front());
                }
                YACK_CHEM_PRINTLN("//    expand=" << expand);

                //--------------------------------------------------------------
                //
                // compute Cend = Corg + expand * dC
                //
                //--------------------------------------------------------------
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    Cend[j] = Corg[j] + expand * dC[j]; assert(Cend[j]>=0);
                }

                {
                    ios::ocstream fp("gam.dat");
                    static const size_t NP=1000;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double u = double(i)/NP;
                        fp("%.15g %.15g\n",u,(*this)(u));
                    }
                }

            }

            //------------------------------------------------------------------
            //
            // Get new value
            //
            //------------------------------------------------------------------
            const double Gtry = optimizeDecreaseFrom(G0);

            YACK_CHEM_PRINTLN("//    G0=" << G0 << " -> " << Gtry);

            //------------------------------------------------------------------
            //
            // don't stop upon numerically fragile result
            //
            //------------------------------------------------------------------
            transfer(Corg,Ctry);
            G0 = Gtry;

            if(cycle>=10)
            {
                //exit(1);
            }

            goto CYCLE;

        }
    }

}


