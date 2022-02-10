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

        double plexus::operator()(const double u)
        {
            make_boundary(Ctry,Corg,u,dC);
            return computeRMS(Ctry);
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
                // ensure maximum regularity
                //
                //
                //--------------------------------------------------------------
                {
                    bool moved = false;
                    ustack.free();
                    for(const enode *node=eqs.head();node;node=node->next)
                    {
                        const equilibrium      &eq  = ***node;
                        const size_t            i   = eq.indx;
                        const readable<double> &psi = Psi[i];

                        //------------------------------------------------------
                        // check acceptable gradient
                        //------------------------------------------------------
                        if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-1

                        moved = true;
                        YACK_CHEM_PRINTLN("//  [moving " << eq.name << "]");
                        eq.solve(K[i],Corg,Ctry);
                        computeGammaAndPsi(Corg);

                        //------------------------------------------------------
                        // check new acceptable gradient
                        //------------------------------------------------------
                        if( tao::v1::mod2<double>::of(psi) > 0) continue; // OK level-2

                        //------------------------------------------------------
                        // register as singular
                        //------------------------------------------------------
                        ustack << i;
                    }

                    if(verbose&&moved) lib(std::cerr << "C0=",Corg);
                }

                //--------------------------------------------------------------
                //
                //
                // initialize status
                //
                //
                //--------------------------------------------------------------
                const double    g0 = computeRMS(Corg);
                triplet<double> x = { 0,0,0 };
                triplet<double> g = { g0,0,0};

                YACK_CHEM_PRINTLN("  block = " << ustack);
                YACK_CHEM_PRINTLN("  Gamma = " << Gamma );
                YACK_CHEM_PRINTLN("  Psi   = " << Psi   );
                YACK_CHEM_PRINTLN("  Nu    = " << Nu    );
                YACK_CHEM_PRINTLN("  G0    = " << g0    );

                if( fabs(g0) <= 0)
                {
                    //----------------------------------------------------------
                    // early return
                    //----------------------------------------------------------
                    tao::v1::set(C,Corg);
                    YACK_CHEM_PRINTLN("// [numerical success]");
                    return;
                }

                //--------------------------------------------------------------
                //
                //
                // computed regularized Jacobian
                //
                //
                //--------------------------------------------------------------
                tao::v3::mmul_trn(W,Psi,Nu);
                for(size_t i=ustack.size();i>0;--i)
                {
                    const size_t k = ustack[i];
                    W[k][k]  = 1;
                    Gamma[k] = 0;
                }
                YACK_CHEM_PRINTLN("  W     = " << W);

                if(!LU.build(W))
                {
                    throw exception("%s: singular concentrations", clid);
                }

                //--------------------------------------------------------------
                //
                //
                // computed Newton's extent
                //
                //
                //--------------------------------------------------------------
                tao::v1::neg(xi,Gamma);
                LU.solve(W,xi);
                YACK_CHEM_PRINTLN("  xi    = " << xi);

                //--------------------------------------------------------------
                //
                //
                // imposing legal direction : xi \propto Gamma
                //
                //
                //--------------------------------------------------------------
                {
                    bool moved=false;
                    for(size_t i=N;i>0;--i)
                    {
                        double &x = xi[i];
                        if(x*Gamma[i]<=0)
                        {
                            x=0;
                            moved = true;
                        }
                    }
                    if(moved) { YACK_CHEM_PRINTLN("  xi    = " << xi); }
                }

                //--------------------------------------------------------------
                //
                //
                // estimating dC
                //
                //
                //--------------------------------------------------------------
                tao::v2::mul(dC,NuT,xi);
                YACK_CHEM_PRINTLN("  dC    = " << dC);


                //--------------------------------------------------------------
                //
                //
                // check possible truncations to ensure positive C
                //
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN("// [truncation]");
                rstack.free();
                ustack.free();
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &sp = ***node;
                    const size_t   j  = sp.indx;
                    const double   d  = dC[j];
                    if(d<0)
                    {
                        YACK_CHEM_PRINTLN("//    checking " << sp);
                        const double b = -d;      assert(b>0);
                        const double c = Corg[j]; assert(c>=0);
                        rstack << c/b;
                        ustack << j;
                    }
                }


                double scale=1;
                size_t count=0;
                if(rstack.size())
                {
                    hsort(rstack,ustack,comparison::increasing<double>);
                    scale = rstack.front();
                    count = 1;
                    while(count<rstack.size() && rstack[count+1] <= scale) ++count;
                    while(rstack.size()>count)
                    {
                        rstack.pop_back();
                        ustack.pop_back();
                    }
                    YACK_CHEM_PRINTLN("// rstack="<<rstack);
                    YACK_CHEM_PRINTLN("// ustack="<<ustack);
                }

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
                        (void) minimize::find<double>::run_for(self, x, g, minimize::inside);
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
                        (void) minimize::find<double>::run_for(self,x,g,minimize::inside);
                        YACK_CHEM_PRINTLN("// [limited @" << scale << " <= 1] x=" << x.b  << ", g=" << g);
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
                            (void) minimize::find<double>::run_for(self,x,g,minimize::inside);
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x.b  << ", g=" << g);
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
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x << ", g=" << g);


                            if(g.c>=g.b)
                            {
                                YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] minimize::direct");
                                (void) minimize::find<double>::run_for(self,x,g,minimize::direct);
                            }
                            else
                            {
                                YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] accept...");
                                x.b = x.c; // save result
                                g.b = g.c; // save result
                            }
                            YACK_CHEM_PRINTLN("// [limited @" << scale << " > 1] x=" << x.b  << ", g=" << g);
                        }

                    }

                }

                //--------------------------------------------------------------
                //
                // prepare for return of next iter
                //
                //--------------------------------------------------------------
                computeGammaAndPsi(Ctry);


                //--------------------------------------------------------------
                //
                // check |Gamma| convergence
                //
                //--------------------------------------------------------------
                const double g1 = g.b;
                YACK_CHEM_PRINTLN("// g0=" << g0 << "; g1=" << g1 << "; dg=" << g1-g0);

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
                    tao::v1::set(C,Ctry);
                    return;
                }


                goto ITER;



            }


        }

    }

}
