

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
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
            ios::ocstream::overwrite("rms.dat");

            switch(N)
            {
                case 0: // trivial case
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

                case 1: { // trivial case
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C,Ctry);
                    transfer(C,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    // prepare workspace
                    tao::v1::load(Ctry,C);
                    break;
            }

            //------------------------------------------------------------------
            //
            // Loop
            //
            //------------------------------------------------------------------
            size_t       cycle = 0;

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // preparation
            //
            //------------------------------------------------------------------
            regularize(C); // perform regularization
            makeOmega0();  // build initial matrix, right hand side and scaling for Gamma


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
                const limits      &lm  = eq.primary_limits(C,lib.width);
                if(verbose)
                {
                    eqs.pad(std::cerr << "//   " << eq.name,eq) << " : " << lm << std::endl;
                }
                if(!lm.is_acceptable(xi[ei]))
                {
                    std::cerr << "//   |_inacceptable" << std::endl;
                    Omega0[ei][ei] *= 2;
                    tao::v1::set(xi,xm);
                    goto EVAL_XI;
                }
            }

            ios::ocstream::echo("rms.dat", "%g %.15g\n", double(cycle), tao::v1::mod2<double>::of(xi)/N);
            
            // dC
            dC.ld(0);
            rstack.free();
            ustack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
                const double   c = C[j]; assert(c>=0);
                if( (d<0) && (d <= -c) )
                {
                    rstack << c/(-d);
                    ustack << j;
                }
            }
            hsort(rstack,ustack,comparison::increasing<double>);
            std::cerr << "C  = " << C  << std::endl;
            std::cerr << "dC = " << dC << std::endl;

            std::cerr << "rstack=" << rstack << " for " << ustack << std::endl;

            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Ctry[j] = max_of( C[j] + dC[j], 0.0);
            }

            transfer(C,Ctry);

            return false;

            if(cycle>=1)
            {
                exit(1);
            }
            goto CYCLE;

            return false;

        }


    }

}
