

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
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << "Cini=",C);
            ios::ocstream::overwrite("rms.dat");

            switch(N)
            {
                case 0:
                    goto SUCCESS;

                case 1: {
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C,Ctry);
                    transfer(C,Ctry);
                } goto SUCCESS;


                default:
                    tao::v1::load(Ctry,C);
                    break;
            }


            {
                size_t cycle = 0;
                rmatrix Omega0(N,N);
                rmatrix iOmega(N,N);
            CYCLE:
                ++cycle;
                const double rms = computeMissing(C);
                ios::ocstream::echo("rms.dat","%g %.15g\n", double(cycle), rms);
                for(size_t i=N;i>0;--i)
                {
                    writable<double>       &Omi = Omega0[i];
                    const readable<double> &psi = Psi[i];
                    double                 &rhs = xi[i];
                    const double            den = xdot(psi,Nu[i],Ctmp);

                    Omi.ld(0);
                    Omi[i] = 1.0;
                    rhs    = 0;
                    if(fabs(den)>0)
                    {
                        for(size_t j=N;j>i;--j)   Omi[j] = xdot(psi,Nu[j],Ctmp)/den;
                        for(size_t j=i-1;j>0;--j) Omi[j] = xdot(psi,Nu[j],Ctmp)/den;
                        rhs  = Xi[i];
                    }
                }
                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "rhs  =" << xi    << std::endl;

                iOmega.assign(Omega0);
                double dampening = 1;

            EVAL_XI:
                if(!LU.build(iOmega))
                {
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                    return false;
                }

                LU.solve(iOmega,xi);
                if(verbose) eqs(std::cerr << "xi0=",xi);

                for(size_t i=N;i>0;--i)
                {
                    if( xi[i] * Xi[i] < 0 )
                    {
                        std::cerr << "\t -------- unsafe --------" << std::endl;
                        dampening *= 10;
                        for(size_t j=N;j>0;--j)
                        {
                            Omega0[j][j] *= dampening;
                            xi[j] = Xi[j];
                        }
                        iOmega.assign(Omega0);
                        goto EVAL_XI;

                    }

                }





                // correcting
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const limits      &lm = eq.primary_limits(C,lib.width);
                    const size_t       ei = *eq;
                    double            &xx = xi[ei];
                    if(verbose)
                    {
                        eqs.pad(std::cerr << " <" << eq.name << ">",eq) << " : " << lm << std::endl;
                    }
                    switch(lm.type)
                    {
                        case limited_by_none:
                            break;

                        case limited_by_reac:
                            xx = min_of(xx,lm.reac_extent());
                            break;

                        case limited_by_prod:
                            xx = max_of(xx,lm.prod_extent());
                            break;

                        case limited_by_both:
                            xx = clamp(lm.prod_extent(),xx,lm.reac_extent());
                            break;

                    }
                }
                if(verbose) eqs(std::cerr << "xi1=",xi);


                ustack.free();
                rstack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   d = dC[j] = xdot(xi,NuT[j],xs);
                    const double   c = C[j]; assert(c>=0);
                    if(d <= -c)
                    {
                        rstack << -c/d;
                        ustack << j;
                    }
                }
                std::cerr << "C="  << C  << std::endl;
                std::cerr << "dC=" << dC << std::endl;


                double     factor = 1.0;
                const bool scaled = rstack.size()>0;
                if(scaled)
                {
                    hsort(rstack,ustack,comparison::increasing<double>);
                    std::cerr << "rstack=" << rstack << std::endl;
                    std::cerr << "ustack=" << ustack << std::endl;
                    factor = min_of(factor,rstack.front());
                }
                std::cerr << "factor=" << factor << std::endl;
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    Ctry[j] = max_of( C[j] + factor * dC[j],0.0);
                }
                if(scaled)
                {
                    Ctry[ ustack.front() ] = 0;
                }

                if(verbose) lib(std::cerr << "Ctry=",Ctry);

                
                transfer(C,Ctry);

                if(cycle>=10)
                {
                    exit(1);
                }

                goto CYCLE;

            }


        SUCCESS:
            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;

        }


    }

}
