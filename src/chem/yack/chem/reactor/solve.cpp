
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/minimize.hpp"
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
        }

        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
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
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D success]");
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

            vector<double> rstack;
            vector<size_t> ustack;

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("//   ---------------- cycle#" << cycle << " ----------------");


            //------------------------------------------------------------------
            //
            // starting point for this cycle
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//    <GlobalStep>");
            double G0 = meanGammaSquared(Corg);

            //------------------------------------------------------------------
            //
            // testing singles and |Xi| convergence
            //
            //------------------------------------------------------------------
            double sumAbsXi = 0;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;     assert(ei>0); assert(ei<=N);
                const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                if(ax>0)
                {
                    sumAbsXi += ax;
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


            if(sumAbsXi<=0)
            {
                transfer(C0,Corg);
                YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS |Xi|=0]");
                return true;
            }

            //------------------------------------------------------------------
            //
            // testing couples
            //
            //------------------------------------------------------------------
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;     assert(ei>N); assert(ei<=Ntot);
                const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                if(ax>0)
                {
                    Gtot[ei]  = optimizeDecreaseFrom(G0);
                    transfer(Ctot[ei],Ctry);
                    couples.pad(std::cerr << vpfx << eq.name,eq) << " : Xi=" << std::setw(15) << Xtot[ei] << " : " << "Gopt=" << std::setw(15)<< Gtot[ei] << std::endl;
                }
                else
                {
                    Gtot[ei] = G0;
                    transfer(Ctot[ei],Corg);
                }
            }
            YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);

            //------------------------------------------------------------------
            //
            // select best single/couple
            //
            //------------------------------------------------------------------
            G0 = selectDecreasedState();
            YACK_CHEM_PRINTLN("//    <GlobalStep/>");

            //------------------------------------------------------------------
            //
            // compute full metrics
            //
            //------------------------------------------------------------------
            size_t num_running = computeOmegaAndGamma();

            //------------------------------------------------------------------
            //
            // evaluate extent
            //
            //------------------------------------------------------------------
        EVALUATE_EXTENT:
            YACK_CHEM_PRINTLN("//    <EvaluateExtent>");
            YACK_CHEM_PRINTLN("//     \\_#running = " << num_running << " / " << N );
            if(verbose)
            {
                eqs(std::cerr << vpfx << "Omega=",Omega0,vpfx);
            }
            // evaluate xi
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("//    <EvaluateExtent/>");
                YACK_CHEM_PRINTLN("// <plexus.solve> [SINGULAR]");
                return false;
            }

            tao::v1::set(xi,Gamma);
            LU.solve(iOmega,xi);
            std::cerr << "xi=" << xi << std::endl;
            std::cerr << "blocked="  << blocked << std::endl;

            //------------------------------------------------------------------
            //
            // check primary limits
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
                    if(verbose) std::cerr << "[blocked]";
                    assert( fabs(xi[ei]) <= 0 );
                }
                else
                {
                    const limits      &lm = eq.primary_limits(Corg,lib.width);
                    if(verbose) std::cerr << lm;
                    if( lm.should_reduce(xx) )
                    {
                        std::cerr << " [reject " << xx << "]";
                        modified = true;
                        --num_running;
                        retractEquilibriumAt(ei);
                    }
                    else
                    {
                        std::cerr << " [accept " << xx << "]";
                    }

                }
                if(verbose) std::cerr << std::endl;
            }
            if(modified)
            {
                YACK_CHEM_PRINTLN("//    <EvaluateExtent/> [modified]");
                goto EVALUATE_EXTENT;
            }
            else
            {
                YACK_CHEM_PRINTLN("//    <EvaluateExtent/> [accepted]");
            }



            //------------------------------------------------------------------
            //
            // try move TODO: check num_running
            //
            //------------------------------------------------------------------
            rstack.free();
            ustack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double d = (dC[j] = sorted::dot(xi,NuTA[j],Xtmp));
                const double c = Corg[j]; assert(c>=0);
                if(d<0)
                {
                    rstack << c/(-d);
                    ustack << j;
                }
            }
            lib(std::cerr << "dC=",dC);
            double expand = 2.0;
            if(rstack.size())
            {
                hsort(rstack,ustack, comparison::increasing<double>);
                // rstack.keep_only_front();
                ustack.keep_only_front();
                expand = min_of(expand,0.99*rstack.front());
            }
            std::cerr << "rstack=" << rstack << std::endl;
            std::cerr << "ustack=" << ustack << std::endl;
            std::cerr << "expand=" << expand << std::endl;

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

            const double G1 = optimizeDecreaseFrom(G0);

            std::cerr << "G0=" << G0 << " -> " << G1 << std::endl;

            exit(1);
            goto CYCLE;
            YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
            return true;
        }
    }

}


