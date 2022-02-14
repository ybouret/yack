#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/sum.hpp"
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
#if 0
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ii = eq.indx;
                    sc[ii] = eq.scale(K[ii],C,Ctmp);
                }
                return  ( sorted::sum_squared(sc)/N );
#endif
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

        

        void plexus:: save_profile(const char *filename, const double umax)  
        {
            ios::ocstream fp(filename);
            YACK_CHEM_PRINTLN("// [saving " << filename << "]");
            for(double u=0;u<1;u+=0.001)
            {
                const double xx = u * umax;
                fp("%.15g %.15g\n", xx, (*this)(xx) );
            }
        }


        double plexus::operator()(const double u)
        {
            make_trial(u);
            return computeVariance(Ctry);
        }


        void plexus:: computeXi()
        {
            //------------------------------------------------------------------
            //
            // Phi*NuT
            //
            //------------------------------------------------------------------
            tao::v3::mmul_trn(W,Psi,Nu);
            for(size_t i=blocked.size();i>0;--i)
            {
                const size_t k = blocked[i];
                W[k][k]  = 1.0;
                Gamma[k] = 0.0;
            }
            YACK_CHEM_PRINTLN("  PsiNuT = " << W);

            //------------------------------------------------------------------
            //
            // inverse
            //
            //------------------------------------------------------------------
            if(!LU.build(W))
            {
                throw exception("%s: singular concentrations", clid);
            }

            //------------------------------------------------------------------
            //
            // xi = -inv(Phi*NuT)*Gamma
            //
            //------------------------------------------------------------------
            tao::v1::neg(xi,Gamma);
            LU.solve(W,xi);
            YACK_CHEM_PRINTLN("  xi     = " << xi);

            //------------------------------------------------------------------
            //
            // validate Xi with primary limits
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const limits      &lm = eq.find_primary_limits(Corg);
                const size_t       ii = eq.indx;
                double            &xx = xi[ii];
                YACK_CHEM_PRINTLN("// @" << ios::align(eq.name,eqs.width) << " : " << xx);
                YACK_CHEM_PRINTLN("// |_" << lm);
                xx = lm.crop(xx);
            }
            YACK_CHEM_PRINTLN("  xi     = " << xi);

        }

        void plexus:: computeDeltaC()
        {
            dC.ld(0);
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species      &sp = ***node;
                const size_t        jj = sp.indx;
                const imatrix::row &nu = NuT[jj];
                for(size_t i=N;i>0;--i) sc[i] = nu[i] * xi[i];
                dC[jj] = sorted::sum(sc, sorted::by_abs_value);
            }

            YACK_CHEM_PRINTLN("  dC     = " << dC);
            YACK_CHEM_PRINTLN("   C     = " << Corg);
        }


        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            //static const double vtol = minimize::get_mtol<double>();

            if(N>0)
            {
                plexus &self = *this;

                //--------------------------------------------------------------
                //
                //
                // initialize for first iteration
                //
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
                //
                // regularize with initial Corg and Psi, setup blocked
                //
                //
                //--------------------------------------------------------------
                if(regularize())
                {
                    if(verbose) lib(std::cerr << "C0=",Corg);
                }

                const double    g0 = computeVariance(Corg);

                YACK_CHEM_PRINTLN("  Gamma  = " << Gamma);
                YACK_CHEM_PRINTLN("  g0     = " << g0);
                YACK_CHEM_PRINTLN("  Psi    = " << g0);
                
                if(g0<=0)
                {

                    YACK_CHEM_PRINTLN("// [numerical success level-1]");
                    tao::v1::set(C,Corg);
                    return;
                }


                //--------------------------------------------------------------
                //
                //
                // compute Newton's extent
                //
                //
                //--------------------------------------------------------------
                computeXi();


                //--------------------------------------------------------------
                //
                //
                // compute associated dC
                //
                //
                //--------------------------------------------------------------
                computeDeltaC();

                double          scale = 1.0;
                const size_t    count = findTruncation(scale);
                triplet<double> x     = { 0,  0, 0 };
                triplet<double> g     = { g0, 0, 0 };

                if(count)
                {
                    //----------------------------------------------------------
                    //
                    //
                    YACK_CHEM_PRINTLN("// [limited]");
                    //
                    //
                    //----------------------------------------------------------
                    


                    exit(1);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    //
                    YACK_CHEM_PRINTLN("// [unlimited]");
                    //
                    //
                    //----------------------------------------------------------
                    save_profile("ulim.dat",1);
                    g.c = g.b = self(x.c = x.b = 1);
                    if(g.b>=g.a)
                    {
                        //------------------------------------------------------
                        //
                        YACK_CHEM_PRINTLN("// [unlimited.backtrack]");
                        //
                        //------------------------------------------------------
                        minimize::find<double>::run_for(self,x,g,minimize::inside);
                        YACK_CHEM_PRINTLN("// [unlimited.backtrack] g=" << g.b << " @" << x.b);

                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        YACK_CHEM_PRINTLN("// [unlimited.forward]");
                        //
                        //------------------------------------------------------
                        do
                        {
                            g.c = self( x.c *= 1.2);
                        } while( g.c<g.b );
                        save_profile("ulim.dat",x.c);
                        minimize::find<double>::run_for(self,x,g,minimize::direct);
                        YACK_CHEM_PRINTLN("// [unlimited.forward] g=" << g.b << " @" << x.b);
                    }

                }

                //--------------------------------------------------------------
                //
                //
                // compute Gamma and Psi at new position, for success of iter
                //
                //
                //--------------------------------------------------------------
                computeGammaAndPsi(Ctry);

                //--------------------------------------------------------------
                //
                //
                // process
                //
                //
                //--------------------------------------------------------------
                const double g1 = g.b;
                if( g1 <= 0)
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
