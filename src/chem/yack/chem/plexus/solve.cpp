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


        double  plexus::  computeVariance(const readable<double> &C)
        {
            assert(C.size()>=M);
            if(N>0)
            {
                computeGamma(C);
                for(size_t i=N;i>0;--i)
                {
                    sc[i] = squared(Gamma[i]);
                }
                return  (sorted::sum(sc,sorted::by_value)/N);
            }
            else
            {
                return 0;
            }

        }

        
        double plexus::operator()(const double u)
        {
            make_trial(u);
            return computeVariance(Ctry);
        }


        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            if(N<=0) return;

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }
            computeGammaAndPsi(Corg);

            YACK_CHEM_PRINTLN("//" << std::endl << "// <solving>");
            size_t iter = 0;
        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//" << std::endl << "// <cycle#" << iter << ">");
            if(verbose) lib(std::cerr << "C0=",Corg);


            //------------------------------------------------------------------
            //
            // regularize
            //
            //------------------------------------------------------------------
            if(regularize()) YACK_CHEM_PRINTLN("  C0     = "<<Corg);


            //------------------------------------------------------------------
            //
            // initial variance
            //
            //------------------------------------------------------------------
            const double g0 = computeVariance(Corg);
            YACK_CHEM_PRINTLN("  g0     = " << g0);
            if( g0 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-1]");
                tao::v1::set(C,Corg);
                return;
            }

            //------------------------------------------------------------------
            //
            // compute Extent and deltaC
            //
            //------------------------------------------------------------------
            computeExtent();
            computeDeltaC();

            //------------------------------------------------------------------
            //
            // compute new status
            //
            //------------------------------------------------------------------
            const double g1 = move(g0);
            YACK_CHEM_PRINTLN("  g1     = " << g1 << " / " << g0);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const species     *vanishing = NULL;
                eqs.pad(std::cerr << "// $" << eq.name,eq.name) << " : " << eq.extent(K[*eq],Ctry,Ctmp,vanishing) << std::endl;
            }
            computeGammaAndPsi(Ctry);

            if(iter>=2)
            {
                std::cerr << "iter = " << iter << std::endl;
                std::cerr << "C    = " << Ctry << std::endl;
                std::cerr << "Psi  = " << Psi << std::endl;
                exit(1);
            }
            if( g1 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-2]");
                tao::v1::set(C,Ctry);
                return;
            }

            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;  if(s.rank<=0) continue;;
                const size_t   j = *s;
                Corg[j] = Ctry[j];
            }

            goto ITER;



        }


    }
}
