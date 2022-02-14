#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"

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
            //static const double vtol = minimize::get_mtol<double>();

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // initialize for first iteration
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    Corg[j] = Ctry[j] = C[j];
                }
                if(verbose) lib(std::cerr << "C0=",Corg);
                computeGammaAndPsi(Corg);
                size_t iter = 0;

            ITER:
                ++iter;
                YACK_CHEM_PRINTLN("//" << std::endl << "// [solve.iter=" << iter << "]");
                //--------------------------------------------------------------
                //
                // regularize with initial Corg and Psi
                // setup blocked equilibria
                //
                //--------------------------------------------------------------
                if(regularize())
                {
                    if(verbose) lib(std::cerr << "C0=",Corg);
                }

                const double    g0 = computeVariance(Corg);

                YACK_CHEM_PRINTLN("  Gamma  = " << Gamma);
                YACK_CHEM_PRINTLN("  g0     = " << g0);
                YACK_CHEM_PRINTLN("  Psi    = " << Psi);

                //--------------------------------------------------------------
                //
                // early return ?
                //
                //--------------------------------------------------------------
                if(g0<=0) {
                    YACK_CHEM_PRINTLN("// [numerical success level-1]");
                    tao::v1::set(C,Corg);
                    return;
                }


                //--------------------------------------------------------------
                //
                // compute Newton's extent
                //
                //--------------------------------------------------------------
                computeXi();


                //--------------------------------------------------------------
                //
                // compute associated dC
                //
                //--------------------------------------------------------------
                computeDeltaC();


                //--------------------------------------------------------------
                //
                // move phase state accordingly
                //
                //--------------------------------------------------------------
                const double g1 = move(g0);


                //--------------------------------------------------------------
                //
                // compute Gamma and Psi at new position, for success of iter
                //
                //--------------------------------------------------------------
                computeGammaAndPsi(Ctry);
                
                //--------------------------------------------------------------
                //
                // process
                //
                //--------------------------------------------------------------
                if(g1 <= 0)
                {
                    YACK_CHEM_PRINTLN("// [numerical success level-2]");
                    tao::v1::set(C,Ctry);
                    return;
                }

                if( g1 >= g0 )
                {
                    YACK_CHEM_PRINTLN("// [numerical convergence]");
                    tao::v1::set(C,Ctry);
                    return;
                }


                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &s = ***node;
                    const size_t   j = s.indx;
                    Corg[j] = Ctry[j];
                }


                goto ITER;
            }


        }

    }

}
