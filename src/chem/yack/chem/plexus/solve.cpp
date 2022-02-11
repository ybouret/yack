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


        double  plexus::  computeRMS(const readable<double> &C)
        {
            assert(C.size()>=M);
            if(N>0)
            {
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ii = eq.indx;
                    sc[ii] = eq.scale(K[ii],C,Ctmp);
                }
                return sqrt( sorted::sum_squared(sc)/N );
            }
            else
            {
                return 0;
            }

        }

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


        double plexus::operator()(const double u)
        {
            make_trial(u);
            return computeRMS(Ctry);
        }

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

        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            static const double vtol = minimize::get_mtol<double>();

            if(N>0)
            {
                plexus &self = *this;
                //--------------------------------------------------------------
                //
                //
                // initialize
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
                // regularize with initial Corg and Psi
                //
                //
                //--------------------------------------------------------------
                if(regularize(ustack))
                {
                    if(verbose) lib(std::cerr << "C0=",Corg);
                }


                //--------------------------------------------------------------
                //
                //
                // initialize status with regularized state
                //
                //
                //--------------------------------------------------------------
                const double    g0 = computeRMS(Corg);
                triplet<double> x  = { 0,  0, 0 };
                triplet<double> g  = { g0, 0, 0 };

                YACK_CHEM_PRINTLN("  block = " << ustack);
                YACK_CHEM_PRINTLN("  Gamma = " << Gamma );
                YACK_CHEM_PRINTLN("  Psi   = " << Psi   );
                YACK_CHEM_PRINTLN("  Nu    = " << Nu    );
                YACK_CHEM_PRINTLN("  G0    = " << g0    );

                if( fabs(g0) <= 0 )
                {
                    //----------------------------------------------------------
                    // early return
                    //----------------------------------------------------------
                    tao::v1::set(C,Corg);
                    YACK_CHEM_PRINTLN("// [numerical success level-1]");
                    return;
                }

                //--------------------------------------------------------------
                //
                //
                // compute search dC
                //
                //
                //--------------------------------------------------------------
                computeDeltaC(ustack);


                //--------------------------------------------------------------
                //
                //
                // check possible truncations to ensure positive C
                //
                //
                //--------------------------------------------------------------
                double       scale = 1;
                const size_t count = findTruncation(scale);

                

                //--------------------------------------------------------------
                //
                //
                // now we need to deal with the possibilities
                //
                //
                //--------------------------------------------------------------
                if(!count)
                {
                    //----------------------------------------------------------
                    //
                    //
                    YACK_CHEM_PRINTLN("// [unlimited]");
                    //
                    //
                    //----------------------------------------------------------
                    g.c = g.b = self(x.c=x.b = 1);

                    if(g.c>=g.a)
                    {
                        // too far
                        YACK_CHEM_PRINTLN("// [unlimited.backtrack] ");
                        minimize::find<double>::run_for(self, x, g, minimize::inside);
                        YACK_CHEM_PRINTLN("// [unlimited.bactrack] x=" << x.b  << "; g=" << g.b);
                    }
                    else
                    {
                        // may accept
                        YACK_CHEM_PRINTLN("// [unlimited.forward]");
                    }

                }
                else
                {
                    //----------------------------------------------------------
                    //
                    //
                    YACK_CHEM_PRINTLN("// [limited]");
                    //
                    //
                    //----------------------------------------------------------
                    if(scale<=1)
                    {
                        //------------------------------------------------------
                        //
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " <= 1] optimize");
                        //
                        //------------------------------------------------------
                        make_boundary(Ctry,Corg,x.c=scale,dC,ustack); // hard boundary
                        g.c = computeRMS(Ctry);                       // right-most value
                        minimize::find<double>::run_for(self,x,g,minimize::inside);
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " <= 1] x=" << x.b  << ", g=" << g.b);
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1]");
                        //
                        //------------------------------------------------------

                        // check full step
                        g.c = g.b = self(x.c=x.b=1);
                        if(g.b >= g.a)
                        {
                            //--------------------------------------------------
                            // not decreased: backtrack
                            //--------------------------------------------------
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] backtrack from 1");
                            minimize::find<double>::run_for(self,x,g,minimize::inside);
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x.b  << ", g=" << g.b);
                        }
                        else
                        {
                            static const double xmax = 2;
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] expanding from 1");

                            if(scale<=xmax)
                            {
                                make_boundary(Ctry,Corg,x.c=scale,dC,ustack); // hard boundary
                                g.c = computeRMS(Ctry);                       // right-most value

                            }
                            else
                            {
                                g.c = self(x.c=xmax);
                            }
                            //YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x.b << ", g=" << g.b);


                            if(g.c>=g.b)
                            {
                                YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] minimize::direct");
                                minimize::find<double>::run_for(self,x,g,minimize::direct);
                            }
                            else
                            {
                                YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] accept...");
                                x.b = x.c; // save result
                                g.b = g.c; // save result
                            }
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x.b  << ", g=" << g.b);
                        }

                    }

                }

                //--------------------------------------------------------------
                //
                // prepare for return of next iter
                //
                //--------------------------------------------------------------
                computeGammaAndPsi(Ctry);
                const double g1 = g.b;

                if( fabs(g1) <= 0 )
                {
                    //----------------------------------------------------------
                    // early return
                    //----------------------------------------------------------
                    tao::v1::set(C,Ctry);
                    YACK_CHEM_PRINTLN("// [numerical success level-2]");
                    return;
                }

                //--------------------------------------------------------------
                //
                // check |Gamma| convergence
                //
                //--------------------------------------------------------------
                const double dg = fabs(g1-g0);
                YACK_CHEM_PRINTLN("// g0=" << g0 << "; g1=" << g1 << "; dg=" << -dg);

                if(iter>100)
                {
                    std::cerr << "too many.."<< std::endl;
                    exit(1);
                }

                // if(dg <= vtol * g0)
                if(g1>=g0)
                {
                    YACK_CHEM_PRINTLN("// [minimum reached @iter=" << iter << "]");
                    tao::v1::set(C,Ctry);
                    return;
                }

                //--------------------------------------------------------------
                //
                // check delta C convergence
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN("// [test convergence]");
                bool converged = true;
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &sp  = ***node;
                    const size_t   j   = sp.indx;
                    const double   d   = fabs(Corg[j]-Ctry[j]);
                    const double   err = vtol * fabs(Ctry[j]);
                    const bool     bad = d>err;
                    if(verbose)
                    {
                        std::cerr << "//   d[" << sp.name << "]";
                        lib.pad(std::cerr,sp.name.size());
                        std::cerr << " : " << std::setw(14) << d;
                        std::cerr << " / " << std::setw(14) << err;
                        std::cerr << " | " << std::setw(14) << Ctry[j];
                        std::cerr << " | (" << (bad?'-':'+') << ")" << std::endl;
                    }
                    if(bad) converged = false;
                    Corg[j] = Ctry[j];
                }

                if(converged)
                {
                    YACK_CHEM_PRINTLN("// [converged @iter=" << iter << "]");
                    tao::v1::set(C,Corg);
                    return;
                }


                goto ITER;
            }


        }

    }

}
