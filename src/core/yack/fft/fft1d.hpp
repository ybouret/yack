
//! \file

#ifndef YACK_FFT_XBITREV_INCLUDED
#define YACK_FFT_XBITREV_INCLUDED 1

#include "yack/setup.hpp"

#include <iostream>
#include <cmath>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //
    //
    //__________________________________________________________________________
    struct fft1d
    {
        template <typename T,const size_t exp2> static
        inline void apply(T data[], const int isign)
        {
            static const size_t size = size_t(1) << exp2;
            {
                static const size_t n = size << 1;
                for(size_t i=1,j=1;i<n;i+=2)
                {
                    if(j>i)
                    {
                        T *lhs = &data[j];
                        T *rhs = &data[i];
                        cswap(lhs[0],rhs[0]);
                        cswap(lhs[1],rhs[1]);
                    }
                    size_t m=size;
                    while( (m>=2) && (j>m) )
                    {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }

            {
                size_t n    = size << 1;
                size_t mmax = 2;
                size_t smax = 1; assert(1<<smax==mmax);
                while (n>mmax)
                {
                    std::cerr << "mmax@" <<  size << ": " << mmax << " 2^" << smax << std::endl;
                    const size_t istep = mmax << 1;
                    const double theta = isign*(6.28318530717959/mmax);
                    double wtemp = sin(0.5*theta);
                    double wpr   = -2.0*wtemp*wtemp;
                    double wpi   = sin(theta);
                    double wr    = 1.0;
                    double wi    = 0.0;
                    for(size_t m=1;m<mmax;m+=2)
                    {
                        for(size_t i=m;i<=n;i+=istep)
                        {
                            const size_t i1=i+1;
                            const size_t j=i+mmax;
                            const size_t j1=j+1;
                            const T tempr = T(wr*data[j]-wi*data[j1]);
                            const T tempi = T(wr*data[j1]+wi*data[j]);
                            data[j]   = data[i]-tempr;
                            data[j1]  = data[i1]-tempi;
                            data[i]  += tempr;
                            data[i1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    ++smax;assert(1<<smax==mmax);
                }
            }
        }
    };

}

#endif

