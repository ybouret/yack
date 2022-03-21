

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




        void plexus:: regularize(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize>");
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
                gam  = eq.grad_action(psi,Ki,C0,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    //----------------------------------------------------------
                    // move to a safer and still valid place
                    //----------------------------------------------------------
                    (void) eq.solve1D(Ki,C0,Ctry);
                    transfer(C0,Ctry);
                    
                    //----------------------------------------------------------
                    // second try
                    //----------------------------------------------------------
                    gam     = eq.grad_action(psi,Ki,C0,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                    }
                }
            }

            if(verbose)
            {
                lib(std::cerr << "//     Cini    = ",C0,"//     ");
                std::cerr <<     "//     blocked = " << blocked << std::endl;
            }

            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }

        void plexus:: makeOmega0() throw()
        {
            // first pass, compute Psi'
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];

                if(blocked[ei])
                {
                    xi[ei] = xm[ei] = Gamma[ei] = 0;
                    psi.ld(0);
                }
                else
                {
                    //xi[ei] = xm[ei] = -(Gamma[ei] = eq.mass_action(Ki,Corg));
                    Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                    eq.drvs_action(psi,Ki,Ci,Ctmp);
                    for(size_t i=M;i>0;--i)
                    {
                        Ctmp[i] = squared(psi[i]);
                    }
                    const double den = sqrt( sorted::sum(Ctmp,sorted::by_value) );
                    for(size_t i=M;i>0;--i)
                    {
                        psi[i]/=den;
                    }
                }
            }

            eqs(std::cerr << "//   Xi = ",Xi,"//   ");
            eqs(std::cerr << "//   UM = ",Psi,"//   ");

            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];

                if(blocked[i])
                {
                    Omi.ld(0);
                    Omi[i] = 1.0;
                }
                else
                {
                    for(size_t k=N;k>0;--k)
                    {
                        Omi[k] = xdot(psi,Nu[k],Ctmp);
                    }
                    xi[i] = xm[i] = Xi[i] * Omi[i];
                }
            }


            YACK_CHEM_PRINTLN("Omega = " << Omega0);
            YACK_CHEM_PRINTLN("rhs   = " << xi);
        }



        bool plexus:: solve(writable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            assert(are_valid(C0));



            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << "//   Cini=",C0,"//   ");
            ios::ocstream::overwrite("rms.dat");
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
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            // Loop
            //
            //------------------------------------------------------------------
            size_t         cycle = 0;

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // preparation
            //
            //------------------------------------------------------------------
            regularize(Corg); // perform regularization of Corg
            makeOmega0();     // compute Omega0 and xi = (xm=-Gamma)




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
            std::cerr << "xi=" << xi << std::endl;
            if(verbose) eqs(std::cerr << "//   xi  = ",xi,"//   ");


            bool changed = false;
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
                const double   c = Corg[j];
                if(d<0 && d <= -c)
                {
                    changed = true;
                    YACK_CHEM_PRINTLN("// need to rescale delta_"<<s.name);
                }
            }
            lib(std::cerr << "dC = ",dC);
            if(changed)
            {
                for(size_t i=N;i>0;--i)
                {
                    Omega0[i][i] *= 10;
                    xi[i]         = xm[i];
                }
                goto EVAL_XI;
            }

            ios::ocstream::echo("rms.dat", "%g %.15g\n", double(cycle), tao::v1::mod2<double>::of(dC)/M);

            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Corg[j] = max_of( Corg[j] + dC[j], 0.0);
            }

            if(cycle>=20)
            {
                exit(1);
            }

            goto CYCLE;


        }


    }

}
