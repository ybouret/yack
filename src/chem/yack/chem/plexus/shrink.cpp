
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

        void plexus:: shrink(double &g0)
        {
            vector<equilibrium *> ev(N,NULL);
            YACK_CHEM_PRINTLN("//" << std::endl << "//   <shrinking>");

            const equilibrium *last = NULL;
            size_t             cycle=0;
            
        TRY_SHRINK:
            ++cycle;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                xs[ei] = ( sc[ei] = eq.extent(K[ei],Corg,Ctmp) );
                ev[ei] = (equilibrium *)&eq;
            }
            hsort(sc,ev,comparison::decreasing_abs<double>);
            if(verbose)
            {
                std::cerr << "//" << std::endl << "//     <excess cycle='" << cycle << "'>" << std::endl;
                for(size_t i=1;i<=N;++i)
                {
                    const equilibrium &eq = *ev[i];
                    eqs.pad(std::cerr << "//       (*) @" << eq.name,eq.name) << " : " << sc[i] << std::endl;
                }
                std::cerr << "//     <excess/>" << std::endl;
            }

            // trying in decreasing order
            for(size_t I=1;I<=N;++I)
            {
                const equilibrium &eq = *ev[I];
                const size_t       ei = *eq;
                YACK_CHEM_PRINTLN("//     <trying " << eq.name << ">");

                tao::v1::set(Ctry,Corg);    // Ctry = Corg
                eq.solve(K[ei],Ctry,Ctmp);  // solved(Ctry)
                const double gt = computeVariance(Ctry);
                YACK_CHEM_PRINTLN("//     |_g=" << gt);

                if(gt<g0)
                {
                    g0 = gt;
                    tao::v1::set(Corg,Ctry);
                    YACK_CHEM_PRINTLN("//     <accept/>");
                    if(verbose)
                    {
                        lib(std::cerr << "C0=",Corg);
                    }
                    last = &eq;
                    goto TRY_SHRINK;
                }
                else
                {
                    // keep Corg
                    YACK_CHEM_PRINTLN("//     <reject/>");

                }
            }



#if 0
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
#endif

            YACK_CHEM_PRINTLN("//   <shrinking/>");

        }

    }

}


