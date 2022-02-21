
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

        void plexus:: computeDeltaC(const readable<double> &xx) throw()
        {
            dC.ld(0);
            for(const anode *node=active.head;node;node=node->next)
            {
                const species      &s = **node;
                const size_t        j = *s;
                const imatrix::row &nu = NuT[j];
                rstack.free();
                for(size_t i=N;i>0;--i)
                {
                    const double x = xx[i];
                    const int    n = nu[i];
                    if(n)
                        rstack.push_back_fast(n*x);
                }
                dC[j] = sorted::sum(rstack,sorted::by_value);
            }
        }

        void plexus:: evolve(writable<double> &C)
        {

            if(N<=0) return;

            //------------------------------------------------------------------
            //
            // fetch initial phase space
            //
            //------------------------------------------------------------------s
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            YACK_CHEM_PRINTLN("// <solving>");
            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }

            //------------------------------------------------------------------
            //
            // compute initial state
            //
            //------------------------------------------------------------------
            computeGammaAndPsi(Corg);
            size_t iter = 0;
        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//" << std::endl << "// <iter=" << iter << ">");
            //------------------------------------------------------------------
            //
            // regularize all
            //
            //------------------------------------------------------------------
            if(regularize()>0 && verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }

            YACK_CHEM_PRINTLN("//   Gamma = " << Gamma);



            //------------------------------------------------------------------
            //
            // compute solving extents
            //
            //------------------------------------------------------------------
            vector<equilibrium *> ev(N,NULL);
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
                std::cerr << "//   <excess>" << std::endl;
                for(size_t i=1;i<=N;++i)
                {
                    eqs.pad(std::cerr << "//     (*) @" << ev[i]->name,ev[i]->name)  << " : " << sc[i] << std::endl;
                }
                std::cerr << "//   <excess/>" << std::endl;
            }

            //------------------------------------------------------------------
            //
            // move most out of eq
            //
            //------------------------------------------------------------------
            const equilibrium &best = *ev.front();
            const size_t       indx = *best;
            YACK_CHEM_PRINTLN("//   <solving " << best.name << "/>");
            best.solve(K[indx],Corg,Ctmp);
            if(verbose) lib(std::cerr << "C0=", Corg);
            computeGammaAndPsi(Corg);
            YACK_CHEM_PRINTLN("//   Gamma = " << Gamma);

            //------------------------------------------------------------------
            //
            // compute predicted extent
            //
            //------------------------------------------------------------------
            tao::v3::mmul_trn(W,Psi,Nu);
            for(size_t i=N;i>0;--i)
            {
                if( blocked[i] )
                {
                    W[i][i]  = 1.0;
                    Gamma[i] = 0.0;
                }
            }

            if(!LU.build(W)) throw exception("%s   singular composition",clid);
            tao::v1::neg(xi,Gamma);
            LU.solve(W,xi);
            YACK_CHEM_PRINTLN("//   xi_p  = " << xi);


#if 0
            // compute predicted dC
            computeDeltaC(xi);
            std::cerr << "dC=" << dC << std::endl;
            // compute correction by best
            {
                const readable<double> &psi = Psi[indx]; assert(M==psi.size());
                const imatrix::row     &nui = Nu[indx];  assert(M==nui.size());
                const double den = tao::v1::dot<double>::of(psi,nui);
                std::cerr << "den_" << best.name << "=" << den << std::endl;
                const double num = -(Gamma[indx]+tao::v1::dot<double>::of(psi,dC));
                std::cerr << "num_" << best.name << "=" << num << std::endl;
                const double xii = num/den;
                std::cerr << "xii_" << best.name << "=" << xii << std::endl;
                xi[indx] += xii;
            }
#endif

            //------------------------------------------------------------------
            //
            // compute corrected extent
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <correcting>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const limits      &lm = eq.find_primary_limits(Corg);
                const size_t       ei = *eq;
                if(verbose)
                {
                    eqs.pad(std::cerr << "//     @" << eq.name,eq.name) << " : " << std::setw(14) << xi[ei] << " | " << lm << std::endl;
                }
                xi[ei] = lm.crop(xi[ei]);
            }
            YACK_CHEM_PRINTLN("//   <correcting/>");
            YACK_CHEM_PRINTLN("//   xi_c  = " << xi);


            //------------------------------------------------------------------
            //
            // compute dC
            //
            //------------------------------------------------------------------
            computeDeltaC(xi);
            YACK_CHEM_PRINTLN("//   dC    = " << dC);


            double          scale = 1.0;
            const size_t    count = truncation(scale);
            triplet<double> x     = {0,0,0};
            triplet<double> g     = {computeVariance(Corg),0,0};
            const double    g0    = g.a;
            YACK_CHEM_PRINTLN("//   g0 = " << g0);
            if(count)
            {
                YACK_CHEM_PRINTLN("//   [limited@" << scale << "]");
                if(scale>1)
                {
                    YACK_CHEM_PRINTLN("//   |_@" << scale << " > 1");
                    save_profile("lim.dat",1);
                    g.c=g.b=(*this)(x.c=x.b=1);
                    if(g.c>=g.a)
                    {
                        // backtrack
                        YACK_CHEM_PRINTLN("//   |_backtrack");
                        minimize::find<double>::run_for(*this,x,g,minimize::inside);
                    }
                    else
                    {
                        // accept
                        YACK_CHEM_PRINTLN("//   |_accept");
                    }

                }
                else
                {
                    YACK_CHEM_PRINTLN("//   |_@" << scale << " <= 1");
                    save_profile("lim.dat",scale);
                    g.c=g.b=(x.c=x.b=scale*0.9);
                    if(g.c>=g.a)
                    {
                        // backtrack
                        YACK_CHEM_PRINTLN("//   |_backtrack");
                        minimize::find<double>::run_for(*this,x,g,minimize::inside);
                    }
                    else
                    {
                        // accept
                        YACK_CHEM_PRINTLN("//   |_accept");
                    }
                }

            }
            else
            {
                YACK_CHEM_PRINTLN("// [unlimited]");
                g.c=g.b=(*this)(x.c=x.b=1);
                save_profile("ulim.dat",1);
                if(g.c>=g.a)
                {
                    // backtrack
                    YACK_CHEM_PRINTLN("// |_backtrack");
                    minimize::find<double>::run_for(*this,x,g,minimize::inside);
                }
                else
                {
                    // accept
                    YACK_CHEM_PRINTLN("// |_accept");
                }
            }
            const double g1 = g.b;
            YACK_CHEM_PRINTLN("//   g1 = " << g1 << " / " << g0);


            bool                converged = true;
            static const double mtol      = minimize::get_mtol<double>();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   del = fabs(Corg[j]-Ctry[j]);
                const double   err = fabs(Ctry[j]) * mtol;
                const bool     bad = del>err;
                if(bad)
                {
                    converged = false;
                }
                Corg[j] = Ctry[j];
            }

            if(converged)
            {
                YACK_CHEM_PRINTLN("// <composition convergence@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }


            if( g1 <= 0)
            {
                YACK_CHEM_PRINTLN("// <numerical success@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }

            if( g1 >= g0)
            {
                YACK_CHEM_PRINTLN("// <variance convergence@iter=" << iter << "/>");
                tao::v1::set(C,Corg);
                return;
            }


            goto ITER;


            //tao::v1::set(C,Corg);


        }




    }

}

