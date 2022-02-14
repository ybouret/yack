#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
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


#if 0
        static inline
        void make_boundary(writable<double>       &Ctry,
                           const readable<double> &Corg,
                           const double            scale,
                           const readable<double> &dC) throw()
        {
            for(size_t j=Ctry.size();j>0;--j)   Ctry[j]  = max_of<double>(0,Corg[j]+scale*dC[j]);
        }



        static inline
        void make_boundary(writable<double>       &Ctry,
                           const readable<double> &Corg,
                           const double            scale,
                           const readable<double> &dC,
                           const readable<size_t> &ustack) throw()
        {
            make_boundary(Ctry,Corg,scale,dC);
            for(size_t j=ustack.size();j>0;--j) Ctry[ ustack[j] ] = 0;
        }
#endif



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
        }

#if 0
        void plexus:: computeDeltaC(const readable<size_t> &blocked)
        {
            //------------------------------------------------------------------
            //
            // Phi*NuT
            //
            //------------------------------------------------------------------
            tao::v3::mmul_trn(W,Psi,Nu);
            for(size_t i=blocked.size();i>0;--i)
            {
                const size_t k = ustack[i];
                W[k][k]  = 1;
                Gamma[k] = 0;
            }
            YACK_CHEM_PRINTLN("  PhiNuT  = " << W);

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
            YACK_CHEM_PRINTLN("  xi    = " << xi);

            //------------------------------------------------------------------
            //
            // imposing legal direction : xi \propto Gamma
            //
            //------------------------------------------------------------------
            {
                bool stuck=false;
                for(size_t i=N;i>0;--i)
                {
                    double &x = xi[i];
                    if(x*Gamma[i]<=0)
                    {
                        x=0;
                        stuck = true;
                    }
                }
                if(stuck) { YACK_CHEM_PRINTLN("  xi    = " << xi); }
            }

            //--------------------------------------------------------------
            //
            //
            // estimating dC = NuT * xi
            //
            //
            //--------------------------------------------------------------
            dC.ld(0);
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species      &sp = ***node;
                const size_t        jj = sp.indx;
                const imatrix::row &nu = NuT[jj];
                for(size_t i=N;i>0;--i) sc[i] = nu[i] * xi[i];
                dC[jj] = sorted::sum(sc, sorted::by_abs_value);
            }

            YACK_CHEM_PRINTLN("  dC    = " << dC);
            YACK_CHEM_PRINTLN("   C    = " << Corg);

        }
#endif

        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            //static const double vtol = minimize::get_mtol<double>();

            if(N>0)
            {
                plexus &self = *this;
                vector<limits*> lim(N,NULL);

                //--------------------------------------------------------------
                //
                //
                // initialize for first iteration
                //
                //
                //--------------------------------------------------------------
                tao::v1::load(Corg,C);
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

                    YACK_CHEM_PRINTLN("//  [numerical success]");
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
                // validate Xi with primary limits
                //
                //
                //--------------------------------------------------------------
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const limits      &lm = eq.find_primary_limits(C);
                    const size_t       ii = eq.indx;
                    double            &xx = xi[ii];
                    YACK_CHEM_PRINTLN("// @" << eq.name << " : " << xx << " | " << lm);
                }



                return;

                goto ITER;
            }


        }

    }

}
