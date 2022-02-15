#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"

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
                // check numerical result
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



                bool converged  = true;
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &s   = ***node;
                    const size_t   j   = s.indx;
                    const double   c   = Ctry[j]; assert(c>=0);
                    const double   d   = fabs( Corg[j] - c);
                    const double   err = fabs( c * numeric<double>::ftol );
                    const bool     bad = (d>err);
                    if(verbose)
                    {
                        lib.pad(std::cerr << "// " << s.name,s.name) << " : " << std::setw(14) << c;
                        std::cerr << " +/- " << std::setw(14) << d;
                        std::cerr << "  /  " << std::setw(14) << err;
                        std::cerr << " | (" << (bad?'-':'+') << ")";
                        std::cerr << std::endl;
                    }


                    if(bad) converged = false;
                    Corg[j] = c;
                }

                if(converged)
                {
                    YACK_CHEM_PRINTLN("// [concentration convergence]");
                    tao::v1::set(C,Corg);
                    return;
                }

                if(iter>100)
                {
                    std::cerr << "  TOO MANY " << std::endl;
                    exit(1);
                }

                goto ITER;
            }


        }

    }

}
