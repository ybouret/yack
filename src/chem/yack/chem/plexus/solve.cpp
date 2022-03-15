

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
                    break;
            }


            {

                (void) computeMissing(C);
                rmatrix Omega(N,N);
                for(size_t i=N;i>0;--i)
                {
                    writable<double>       &Omi = Omega[i];
                    const readable<double> &psi = Psi[i];
                    double                 &rhs = xi[i];
                    const double            den = tao::v1::mod2<double>::of(psi);
                    if(den>0)
                    {
                        for(size_t j=N;j>0;--j)
                        {
                            Omi[j] = xdot(psi,Nu[j],Ctmp);
                        }
                        rhs  = xdot(psi,Ceq[i],Ctmp);
                        rhs -= xdot(psi,C,Ctmp);
                    }
                    else
                    {
                        Omi[i] = 1.0;
                        rhs    = 0;
                    }

                }
                std::cerr << "Omega=" << Omega << std::endl;
                std::cerr << "rhs  =" << xi    << std::endl;

                if(!LU.build(Omega))
                {
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                    return false;
                }

                LU.solve(Omega,xi);
                std::cerr << "xi   =" << xi    << std::endl;

                // correcting
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const limits      &lm = eq.primary_limits(C,lib.width);
                    if(verbose)
                    {
                        eqs.pad(std::cerr << " <" << eq.name << ">",eq) << " : " << lm << std::endl;
                    }
                }

                exit(1);

            }



        SUCCESS:
            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;

        }


    }

}
