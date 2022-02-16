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
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            vector<size_t>       corrected(N,as_capacity);
            vector<equilibrium*> estack;

            plexus &self = *this;
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }
            computeGammaAndPsi(Corg);
            size_t iter = 0;


            //------------------------------------------------------------------
            //
            //
            // Start iteration
            //
            //
            //------------------------------------------------------------------
        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//" << std::endl << "// <solving#" << iter << ">");
            //------------------------------------------------------------------
            //
            // regularizing by checking gradients
            //
            //------------------------------------------------------------------
            {
                const size_t moved = regularize();
                if(moved && verbose)
                {
                    YACK_CHEM_PRINTLN("// moved #" << moved);
                    if(blocked.size()>0) std::cerr << "// blocked=" << blocked << std::endl;
                }
            }
            if(verbose) lib(std::cerr<<"C0=",Corg);

        //RESTART:
            const double g0 = computeVariance(Corg);

            if( g0 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-1]");
                tao::v1::set(C,Corg);
                return;
            }

            //------------------------------------------------------------------
            //
            // compute Xi
            //
            //------------------------------------------------------------------
            computeXi();


            //------------------------------------------------------------------
            //
            // check compatible descent
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// [descent]");
            bool stopped = false;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                const double       xx = xi[i];
                const double       gx = Gamma[i];
                const bool         ok = (gx*xx >= 0);
                YACK_CHEM_PRINTLN("//   @" << ios::align(eq.name,eqs.width)
                                  << " = " << std::setw(14) << xx
                                  << " | gamma = " << std::setw(14) << gx
                                  << " | " << (ok?'+':'-') );
                if(!ok)
                {
                    xi[i]   = 0;
                    stopped = true;
                }
            }

            if(stopped)
            {
                YACK_CHEM_PRINTLN("// [descent] altering composition");
                YACK_CHEM_PRINTLN("  xi     = " << xi);
            }
            else
            {
                YACK_CHEM_PRINTLN("// [descent] consistent composition");
            }



            //------------------------------------------------------------------
            //
            // computing and checking dC, rstack and ustack are rebuilt
            //
            //------------------------------------------------------------------
            computeDeltaC();
            double          scale = 1;
            const size_t    count = truncation(scale);
            triplet<double> x     = { 0,  0, 0 };
            triplet<double> g     = { g0, 0, 0 };
            if(count>0)
            {
                //--------------------------------------------------------------
                //
                YACK_CHEM_PRINTLN("// [limited@" << scale << "]");
                //
                //------------------------------------------------------------------
                save_profile("lim.dat",min_of(scale,2.0));
                if(scale <= 1)
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [limited@" << scale << " <= 1]");
                    //----------------------------------------------------------
                    g.c = computeVariance( make_trial(x.c=scale,ustack) );
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                    if(x.b>=scale)
                    {
                        // recompute
                        g.b = computeVariance( make_trial(x.b=scale,ustack) );
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [limited@" << scale << " >  1]");
                    //----------------------------------------------------------
                    g.c=g.b=self(x.b=x.c=1);
                    if(g.c>=g.a)
                    {
                        YACK_CHEM_PRINTLN("// |_backtrack");
                        minimize::find<double>::run_for(self,x,g,minimize::inside);
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN("// |_accept");
                    }
                }

            }
            else
            {
                //--------------------------------------------------------------
                //
                YACK_CHEM_PRINTLN("// [unlimited]");
                //
                //--------------------------------------------------------------
                g.c = self(x.c=1);
                if(g.c<g.a)
                {
                    //save_profile("ulim.dat",1.2);
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [unlimited] forward");
                    //----------------------------------------------------------
                    g.b = self( x.b = 0.5);
                    minimize::find<double>::run_for(self,x,g,minimize::expand);
                }
                else
                {
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// [unlimited] backtrack");
                    //----------------------------------------------------------
                    //save_profile("ulim.dat",1);
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                }

            }

            // new value @Ctry: compute for next iter or return
            tao::v1::set(xi,Gamma);
            computeGammaAndPsi(Ctry);
            const double g1 = g.b;
            const double dg = g1-g0;
            YACK_CHEM_PRINTLN("// g1=" << g1 << " (dg=" << dg << ") @Ctry=" << Ctry);
            YACK_CHEM_PRINTLN("// Gamma: " << xi << " -> " << Gamma);

            if( g1 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-2]");
                tao::v1::set(C,Ctry);
                return;
            }



            if( g1 >= g0 )
            {
                YACK_CHEM_PRINTLN("// [numerical minimum @" << g1 << "]");
                tao::v1::set(C,Ctry);
                return;
            }


            YACK_CHEM_PRINTLN("// [updating]");
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank<=0)
                {
                    assert(fabs(s(dC)<=0));
                    continue;
                }
                const size_t   j   = s.indx;
                const double   c   = Ctry[j]; assert(c>=0);
                const double   d   = fabs(c-Corg[j]);
                const double   err = fabs(numeric<double>::ftol * c);
                const bool     bad = d>err;
                if(verbose)
                {
                    const char ch = bad ? '-':'+';
                    lib.pad(std::cerr << "// (" << ch << ") d[" << s.name << "]",s.name) ;
                    std::cerr << " : " << std::setw(14) << d;
                    std::cerr << " / " << std::setw(14) << err;
                    std::cerr << " @"  << c;
                    std::cerr << std::endl;
                }
                Corg[j] = c;
            }


            goto ITER;

        }

    }

}
