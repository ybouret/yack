

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sort/sum.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"

#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool plexus::verbose = false;

        plexus:: ~plexus() throw() {}

        static const char fn[] = "chemical::plexus";

        static inline
        size_t check(const library &lib, const equilibria &eqs)
        {
            if(lib.size()<eqs.size())
            {
                throw exception("%s: too many equilibria",fn);
            }
            return lib.size();
        }

        plexus:: plexus(library &lib_, equilibria &eqs_) :
        lib(lib_),
        eqs(eqs_),
        N(eqs.size()),
        M(check(lib,eqs)),
        A(lib.active()),
        ntab(10,N),
        mtab(10,M),

        K(ntab.next()),
        Gamma(ntab.next()),

        Corg(mtab.next()),
        Ctry(mtab.next()),
        dC(mtab.next()),
        xi(ntab.next()),

        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        W(N,N),
        rstack(M,as_capacity),
        ustack(M,as_capacity),

        LU(N),

        lib_lock( coerce(lib) )
        {
            YACK_CHEM_PRINTLN("#species=" << M );
            YACK_CHEM_PRINTLN("#active =" << A );
            YACK_CHEM_PRINTLN("#eqs    =" << N );

            //------------------------------------------------------------------
            //
            // initialize Nu
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.validate();
                eq.fill(coerce(Nu[eq.indx]));
            }

            //------------------------------------------------------------------
            //
            // finalize
            //
            //------------------------------------------------------------------
            coerce(NuT).assign(Nu,transposed);

            //------------------------------------------------------------------
            //
            // check equilibria independence
            //
            //------------------------------------------------------------------
            if(N>0)
            {
                matrix<apq> G(N,N);
                tao::v3::gram(G,Nu);
                YACK_CHEM_PRINTLN( "Nu  = " << Nu );
                YACK_CHEM_PRINTLN( "NuT = " << NuT);
                YACK_CHEM_PRINTLN( "G   = " << G  );

                lu<apq> alu(N);
                if(!alu.build(G))
                    throw exception("%s: dependant equilibria detected",fn);

            }

        }

        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[eq.indx] = eq.K(t);
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                Gamma[i] = eq.mass_action(K[i],C);
            }
        }

        void plexus:: computeGammaAndPsi(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                Gamma[i] = eq.drvs_action(Psi[i],K[i],C);
            }
        }


        double plexus:: objectiveGamma() throw()
        {
            rstack.free();
            for(size_t i=N;i>0;--i)
            {
                rstack.push_back( squared(Gamma[i]) );
            }
            return sorted::sum(rstack,sorted::by_value)*0.5;
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
            computeGamma(Ctry);
            return objectiveGamma();
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
                YACK_CHEM_PRINTLN("// [solve.iter=" << iter << "]");
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

                    if(verbose&&moved) lib(std::cerr << "Corg=",Corg);
                }

                //--------------------------------------------------------------
                //
                //
                // initialize status
                //
                //
                //--------------------------------------------------------------
                const double    g0 = objectiveGamma();
                triplet<double> x = { 0,0,0 };
                triplet<double> g = { g0,0,0};

                YACK_CHEM_PRINTLN("block = " << ustack);
                YACK_CHEM_PRINTLN("Gamma = " << Gamma);
                YACK_CHEM_PRINTLN("Psi   = " << Psi);
                YACK_CHEM_PRINTLN("Nu    = " << Nu  );
                YACK_CHEM_PRINTLN("G0    = " << g.a );
                
                if( fabs(g.a) <= 0)
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
                YACK_CHEM_PRINTLN("W     = " << W);

                if(!LU.build(W))
                {
                    throw exception("%s: singular concentrations", fn);
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
                YACK_CHEM_PRINTLN("xi    = " << xi);

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
                    if(moved) { YACK_CHEM_PRINTLN("xi    = " << xi); }
                }

                //--------------------------------------------------------------
                //
                //
                // estimating dC
                //
                //
                //--------------------------------------------------------------
                tao::v2::mul(dC,NuT,xi);
                YACK_CHEM_PRINTLN("dC    = " << dC);


                //--------------------------------------------------------------
                //
                //
                // check possible truncations to ensure positive C
                //
                //
                //--------------------------------------------------------------
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
                    YACK_CHEM_PRINTLN("rstack="<<rstack);
                    YACK_CHEM_PRINTLN("ustack="<<ustack);
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
                    YACK_CHEM_PRINTLN("// [unlimited]");
                    //
                    //----------------------------------------------------------
                    g.b = self(x.b = 1);
                    g.c = self(x.c = 2);
                    (void) minimize::find<double>::run_for(self, x, g, minimize::expand);
                    YACK_CHEM_PRINTLN("// [unlimited] x=" << x.b << "; g=" << g.b);
                    YACK_CHEM_PRINTLN("// [unlimited] Ctry=" << Ctry << "; Gamma=" << Gamma);


                }
                else
                {
                    //----------------------------------------------------------
                    //
                    YACK_CHEM_PRINTLN("// [limited] @" << scale);
                    //
                    //----------------------------------------------------------

                    if(scale<=1)
                    {
                        YACK_CHEM_PRINTLN("// [limited] @" << scale << " <= 1");
                        //------------------------------------------------------
                        // prepare hard boundary
                        //------------------------------------------------------
                        x.c = scale;
                        make_boundary(Ctry,Corg,scale,dC,ustack);
                        computeGamma(Ctry);
                        g.c = objectiveGamma();

                        //------------------------------------------------------
                        // backtrack
                        //------------------------------------------------------
                        (void) minimize::find<double>::run_for(self, x, g, minimize::inside);
                        YACK_CHEM_PRINTLN("// [limited<=1] x=" << x.b << "; g=" << g.b);
                        YACK_CHEM_PRINTLN("// [limited<=1] Ctry=" << Ctry << "; Gamma=" << Gamma);
                    }
                    else
                    {
                        YACK_CHEM_PRINTLN("// [limited] @" << scale << " > 1");
                        //------------------------------------------------------
                        // prepare soft boundary
                        //------------------------------------------------------
                        x.c = x.b = 1;
                        make_boundary(Ctry,Corg,scale,dC);
                        computeGamma(Ctry);
                        g.c = g.b = objectiveGamma();

                        //YACK_CHEM_PRINTLN("// [limited>1] x=" << x.b << "; g=" << g.b);
                        //------------------------------------------------------
                        // backtrack if not decreased
                        //------------------------------------------------------
                        if(g.c>=g.a)
                        {
                            (void) minimize::find<double>::run_for(self, x, g, minimize::inside);
                        }
                        YACK_CHEM_PRINTLN("// [limited>1] x=" << x.b << "; g=" << g.b);
                        YACK_CHEM_PRINTLN("// [limited>1] Ctry=" << Ctry << "; Gamma=" << Gamma);

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
                if(g.b>=g0)
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
                bool converged = true;
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &sp = ***node;
                    const size_t   j  = sp.indx;
                    const double   d  = fabs(Corg[j]-Ctry[j]);
                    //std::cerr << "delta." << sp.name << " : " << d << "/" << Ctry[j] << std::endl;

                }
                tao::v1::set(Corg,Ctry);

                goto ITER;



            }

            
        }


    }
    
}

