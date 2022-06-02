
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
            // evaluate xi
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//    <EvaluateStep>");
            YACK_CHEM_PRINTLN("//     \\_#running = " << num_running);

            // evaluate xi
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("//    <EvaluateStep/>");
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
            for(const enode *node = eqs.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       xx = xi[ei];
                if(verbose) eqs.pad(std::cerr << "// (*) " << eq.name,eq) << " | " << std::setw(15) << xx << " | ";
                if(blocked[ei])
                {
                    if(verbose) std::cerr << "[blocked]";
                }
                else
                {
                    const limits      &lm = eq.primary_limits(Corg,lib.width);
                    if(verbose) std::cerr << lm;
                }
                if(verbose) std::cerr << std::endl;
            }



            exit(1);
            goto CYCLE;
            YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
            return true;
        }
    }

}


