

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/math/opt/minimize.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {


        template <typename T>
        static inline double xdot(const readable<double> &lhs, const readable<T> &rhs, writable<double> &tmp)
        {
            const size_t n = lhs.size();
            for(size_t i=n;i>0;--i)
            {
                tmp[i] = lhs[i] * rhs[i];
            }
            return sorted::sum(tmp,sorted::by_abs_value);
        }




        void plexus:: regularize(writable<double> &C) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize>");
            bool changed = false;
            blocked.ld(false);

            //------------------------------------------------------------------
            //
            // check each equilibrium
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                double            &gam = Gamma[ei];

                //--------------------------------------------------------------
                // first try
                //--------------------------------------------------------------
                gam  = eq.grad_action(psi,Ki,C,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    //----------------------------------------------------------
                    // move to a safer and still valid place
                    //----------------------------------------------------------
                    (void) eq.solve1D(Ki,C,Ctry);
                    transfer(C,Ctry);
                    changed = true;

                    //----------------------------------------------------------
                    // second try
                    //----------------------------------------------------------
                    gam     = eq.grad_action(psi,Ki,C,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                    }
                }
            }

            if(verbose)
            {
                lib(std::cerr << "//     Cini    = ",C,"//     ");
                std::cerr <<     "//     blocked = " << blocked << std::endl;
                std::cerr <<     "//     changed = " << changed << std::endl;
                eqs(std::cerr << "//     Gamma   = ",Gamma,"//     ");
                eqs(std::cerr << "//     Psi     = ",Psi,  "//     ");
            }

            //------------------------------------------------------------------
            //
            // update if needed
            //
            //------------------------------------------------------------------
            if(changed)
            {
                computeState(C);
            }

            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }

        void plexus:: makeOmega0() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];

                if(blocked[i])
                {
                    Omi.ld(0);
                    Omi[i] = 1;
                    xi[i]  = xm[i] = 0;
                    Gs[i]  = 1;
                }
                else
                {
                    for(size_t j=N;j>0;--j)
                    {
                        Omi[j] = xdot(psi,Nu[j],Ctmp);
                    }
                    xi[i] = (xm[i]=-Gamma[i]);
                    Gs[i] = -Omi[i];
                }
            }
            
            YACK_CHEM_PRINTLN("Omega = " << Omega0);
            YACK_CHEM_PRINTLN("rhs   = " << Gamma);
            YACK_CHEM_PRINTLN("Gs    = " << Gs);
        }


        double plexus:: rmsOfGamma() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                xs[i] = blocked[i] ? 0 : squared(Gamma[i]/Gs[i]);
            }
            return sorted::sum(xs,sorted::by_value) / N;
        }

        double plexus:: rmsOfGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            return rmsOfGamma();
        }

        double plexus:: operator()(const double u) throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Ctry[j] = max_of( Corg[j] + u * dC[j], 0.0);
            }
            return rmsOfGamma(Ctry);
        }

        bool plexus:: solve(writable<double> &C) throw()
        {
            assert(C.size()>=M);
            assert(are_valid(C));



            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << "//   Cini=",C,"//   ");

            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C,Ctry);
                    transfer(C,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = C[j];
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            // Loop
            //
            //------------------------------------------------------------------
            size_t       cycle = 0;
            plexus      &self  = *this;

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // preparation
            //
            //------------------------------------------------------------------
            regularize(Corg); // perform regularization of Corg
            makeOmega0();     // build initial matrix, right hand side and scaling for Gamma


        EVAL_XI:
            //------------------------------------------------------------------
            //
            // use secondary matrix for inversion
            //
            //------------------------------------------------------------------
            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                return false;
            }

            //------------------------------------------------------------------
            //
            // compute step from pre-loaded right hand side in xi
            //
            //------------------------------------------------------------------
            LU.solve(iOmega,xi);
            eqs(std::cerr << "//   xi  = ",xi,"//   ");


            //------------------------------------------------------------------
            //
            // step control function
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const limits      &lm  = eq.primary_limits(Corg,lib.width);
                if(verbose)
                {
                    eqs.pad(std::cerr << "//   " << eq.name,eq) << " : " << lm << std::endl;
                }
                if(!lm.is_acceptable(xi[ei]))
                {
                    std::cerr << "//   |_inacceptable <" << eq.name << "> = " << xi[ei] << std::endl;
                    Omega0[ei][ei] *= 10;
                    tao::v1::set(xi,xm);
                    goto EVAL_XI;
                }
            }


            // dC
            dC.ld(0);
            rstack.free();
            ustack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
                const double   c = Corg[j]; assert(c>=0);
                if( (d<0)
                   //&& (d <= -c)
                   )
                {
                    rstack << c/(-d);
                    ustack << j;
                }
            }
            hsort(rstack,ustack,comparison::increasing<double>);
            std::cerr << "C0 = " << Corg  << std::endl;
            std::cerr << "dC = " << dC   << std::endl;
            
            std::cerr << "rstack=" << rstack << " for " << ustack << std::endl;

            double scale = rstack.size() ? rstack.front() : 1.0;

            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Ctry[j] = max_of( Corg[j] + dC[j], 0.0);
            }

            const double    g0 = rmsOfGamma();
            const double    g1 = rmsOfGamma(Ctry);

            {
                ios::ocstream fp("rms.dat");
                const double umax = min_of(scale,1.5);
                for(double u=0;u<=umax;u+=0.001)
                {
                    fp("%g %g\n", u, self(u) );
                }
            }

            std::cerr << "g0=" << g0 << ", g1=" << g1 << std::endl;
            triplet<double> x = {  0, -1,  1 };
            triplet<double> g = { g0, -1, g1 };
            minimize::find<double>::run_for(self,x,g,minimize::inside);


            std::cerr << " => g=" << g.b << " @" << x.b << std::endl;

            if(g.b>=g0)
            {
                std::cerr << "converged!" << std::endl;
                transfer(C,Ctry);
                computeState(C);
                return true;
            }



            transfer(Corg,Ctry);


            goto CYCLE;

            return false;

        }


    }

}
