
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {



        void plexus:: solve(writable<double> &C)
        {

            switch(N)
            {
                case 0: return;
                case 1: {
                    const equilibrium &eq = ***(eqs.head());
                    eq.solve(K[1],C,Ctmp);
                    computeGammaAndPsi(C);
                } return;

                default:
                    break;
            }
            assert(N>=2);

            //------------------------------------------------------------------
            //
            // fetch initial phase space
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            YACK_CHEM_PRINTLN("// <solving>");
            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }


            vector<equilibrium *> ev(N,NULL);
            double                g0 = computeVariance(Corg);
            YACK_CHEM_PRINTLN("//   g0=" << g0);

            //------------------------------------------------------------------
            //
            // reduction
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//" << std::endl << "//   <shrinking>");
            while(true)
            {
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    sc[ei] = eq.extent(K[ei],Corg,Ctmp);
                    ev[ei] = (equilibrium *)&eq;
                }
                hsort(sc,ev,comparison::decreasing_abs<double>);
                if(verbose)
                {
                    std::cerr << "//     <excess>" << std::endl;
                    for(size_t i=1;i<=N;++i)
                    {
                        const equilibrium &eq = *ev[i];
                        eqs.pad(std::cerr << "//       (*) @" << eq.name,eq.name) << " : " << sc[i] << std::endl;
                    }
                    std::cerr << "//     <excess/>" << std::endl;
                }

                const equilibrium &best = *ev[1];
                const size_t       indx = *best;
                YACK_CHEM_PRINTLN("//     <trying " << best.name << ">");
                tao::v1::set(Ctry,Corg);
                best.solve(K[indx],Ctry,Ctmp);
                const double gt = computeVariance(Ctry);
                YACK_CHEM_PRINTLN("//     gt=" << gt);
                if(gt<g0)
                {
                    g0 = gt;
                    tao::v1::set(Corg,Ctry);
                    YACK_CHEM_PRINTLN("//     <accept/>");
                    continue;
                }
                else
                {
                    // keep Corg
                    YACK_CHEM_PRINTLN("//     <reject/>");
                    break;
                }
            }
            YACK_CHEM_PRINTLN("//   <shrinking/>");


            // full differential state
            computeGammaAndPsi(Corg);

            if( regularize() && verbose )
            {
                lib(std::cerr << "C0=", Corg);

            }

            computeXi();



            exit(1);
        }

    }

}


