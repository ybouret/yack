
#include "yack/chem/plexus.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>

namespace yack
{

    namespace chemical
    {


       

        static inline
        double xdot(const readable<double> &psi,
                    const readable<int>    &nu,
                    const size_t            M,
                    writable<double>       &arr)
        {
            for(size_t j=M;j>0;--j)
            {
                arr[j] = psi[j] * nu[j];
            }
            return sorted::sum(arr,sorted::by_abs_value);
        }

        void plexus:: computeOmega0()  throw()
        {
            Omega0.ld(0);
            
            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i]; Omi[i] = 1;
                const readable<double> &psi = Psi[i];
                const double            den = xdot(psi,Nu[i],M,Ctmp);
                if( fabs(den)>0 )
                {
                    for(size_t k=N;k>i;--k)
                    {
                        Omi[k] = xdot(psi,Nu[k],M,Ctmp);
                    }
                    for(size_t k=i-1;k>0;--k)
                    {
                        Omi[k] = xdot(psi,Nu[k],M,Ctmp);
                    }
                }
                else
                {
                    Xi[i] = 0;
                }
            }
        }
        
    }

}

