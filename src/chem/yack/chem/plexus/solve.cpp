

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



        bool plexus:: solve(writable<double> &C) throw()
        {
            assert(C.size()>=M);
            assert(are_valid(C));
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << "Cini=",C);
            ios::ocstream::overwrite("rms.dat");

            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

                case 1: {
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C,Ctry);
                    transfer(C,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    tao::v1::load(Ctry,C);
                    break;
            }

            rmatrix      Omega0(N,N);
            rmatrix      iOmega(N,N);
            vector<bool> blocked(N,false);
            size_t       cycle = 0;

        CYCLE:
            ++cycle;
            blocked.ld(false);
            bool         changed = false;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                double            &gam = Gamma[ei];

                gam  = eq.grad_action(psi,Ki,C,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    (void) eq.solve1D(Ki,C,Ctry);
                    transfer(C,Ctry);
                    changed = true;
                    gam = eq.grad_action(psi,Ki,C,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                    }
                }
            }
            std::cerr << "blocked=" << blocked << std::endl;
            std::cerr << "changed=" << changed << std::endl;
            if(verbose) lib(std::cerr << "Cini=",C);

            if(changed)
            {
                computeState(C);
            }

            eqs(std::cerr << "Gamma=",Gamma);
            eqs(std::cerr << "Psi  =",Psi);

            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];

                if(blocked[i])
                {
                    Omi.ld(0);
                    Omi[i] = 1;
                    xi[i]  = 0;
                }
                else
                {
                    //const double den = xdot(psi,Nu[i],Ctmp); assert(den<0);
                    //for(size_t j=N;j>i;--j)   Omi[j] = xdot(psi,Nu[j],Ctmp)/den;
                    //for(size_t j=i-1;j>0;--j) Omi[j] = xdot(psi,Nu[j],Ctmp)/den;
                    for(size_t j=N;j>0;--j)
                    {
                        Omi[j] = xdot(psi,Nu[j],Ctmp);
                    }
                    xi[i] = (Gamma[i]=-Gamma[i]);
                }
            }

            std::cerr << "Omega=" << Omega0 << std::endl;
            std::cerr << "rhs  =" << xi     << std::endl;

        EVAL_XI:
            iOmega.assign(Omega0);
            
            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                return false;
            }

            LU.solve(iOmega,xi);
            eqs(std::cerr << "xi  =",xi);

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
                    tao::v1::set(xi,Gamma);
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

            if(cycle>=20)
            {
                exit(1);
            }
            goto CYCLE;

            return false;

        }


    }

}
