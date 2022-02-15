
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


        void plexus:: solve2(writable<double> &C)
        {

            if(N<=0) return;

            plexus &self = *this;
            // initialize

            YACK_CHEM_PRINTLN("// [solving] ");
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }
            computeGammaAndPsi(Corg);
            if(verbose) lib(std::cerr<<"C0=",Corg);

            size_t iter =0;

        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("// [solving.iter=" << iter << "] ");

            // regularize
            if( regularize() )
            {
                if(verbose) lib(std::cerr<<"C0=",Corg);
            }
            const double g0 = computeVariance(Corg);
            YACK_CHEM_PRINTLN("  Gamma  = " << Gamma);
            YACK_CHEM_PRINTLN("  Psi    = " << Psi);
            YACK_CHEM_PRINTLN("  NuT    = " << NuT);
            YACK_CHEM_PRINTLN("  g0     = " << g0);

            
            if(g0<=0)
            {
                tao::v1::set(C,Corg);
                YACK_CHEM_PRINTLN("// [numerical convergence level-1]");
                return;
            }

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                std::cerr << eq.name << " @" << eq.scale(K[eq.indx],Corg,Ctmp) << std::endl;
            }


            // compute xi
            computeXi();

            // compute dC from Xi
            computeDeltaC();

            // checking dC
            double          scale = 1;
            const size_t    count = truncation(scale);
            triplet<double> x     = { 0,  0, 0 };
            triplet<double> g     = { g0, 0, 0 };

            return;

            if(count && scale<=1 )
            {
                YACK_CHEM_PRINTLN("// [trunc]");
                g.b = g.c = computeVariance( make_trial(x.b=x.c=scale,ustack) );
                save_profile("trunc.dat",x.c);
                if(g.c>=g.a)
                {
                    YACK_CHEM_PRINTLN("// [trunc] backtrack");
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                    if(x.b>=scale)
                    {
                        YACK_CHEM_PRINTLN("// [trunc] recompute");
                        g.b = computeVariance( make_trial(x.b=scale,ustack) );
                    }
                }
                YACK_CHEM_PRINTLN("// [trunc] g=" << g.b << " @" << x.b);
            }
            else
            {
                YACK_CHEM_PRINTLN("// [plain]");
                g.b = g.c = self(x.b=x.c=1);
                save_profile("plain.dat",x.c);
                if(g.c>=g.a)
                {
                    YACK_CHEM_PRINTLN("// [plain] backtrack");
                    minimize::find<double>::run_for(self,x,g,minimize::inside);
                }
                else
                {
                    YACK_CHEM_PRINTLN("// [plain] accept");
                }
                YACK_CHEM_PRINTLN("// [plain] g=" << g.b << " @" << x.b);
            }

            // update metrics for next iter or exit
            computeGammaAndPsi(Ctry);
            const double g1 = g.b;

            if(g0<=0)
            {
                YACK_CHEM_PRINTLN("// [numerical convergence level-2]");
                tao::v1::set(C,Ctry);
                return;
            }

            if(g1>=g0)
            {
                YACK_CHEM_PRINTLN("// [converged variance]");
                tao::v1::set(C,Ctry);
                return;
            }

            // advancing
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = s.indx;
                const double   c = Ctry[j]; assert(c>=0);
                const double   d = fabs(Corg[j]-Ctry[j]);
                if(verbose)
                {
                    lib.pad(std::cerr << "// [" << s.name << "]",s.name) << " : " << std::setw(14) << c;
                    std::cerr << " +/- " << std::setw(14) << d;
                    std::cerr << std::endl;
                }
                Corg[j] = c;
            }


            goto ITER;

        }

    }

}


