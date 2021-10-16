
//! \file

#ifndef YACK_FFT_XBITREV_INCLUDED
#define YACK_FFT_XBITREV_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/type/constants.hpp"

#define  YACK_FFT_TRACK
#if defined(YACK_FFT_TRACK)
#include "yack/system/wtime.hpp"
#endif

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
        static uint64_t     algo_ticks;
        static const size_t one      = 1;
        static const size_t max_exp2 = (sizeof(size_t)<<3) - 1;
        static const size_t max_size = one << max_exp2;
        static const double pos_sine[64]; //!< sin(pi/(2^indx))
        static const double neg_sine[64]; //!< -sin(pi/(2^indx))
        
        template <typename T> static
        inline void forward(T data[], const size_t size) throw()
        {
            apply(data,size,pos_sine);
        }
        
        
        template <typename T> static
        inline void reverse(T data[], const size_t size) throw()
        {
            apply(data,size,neg_sine);
        }
        
    private:
        
        template <typename T> static
        inline void apply(T            data[],
                          const size_t size,
                          const double *sine) throw()
        {
            //const int isign = (sine==pos_sine) ? 1 : -1;
            const size_t n = (size << 1);
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

#if defined(YACK_FFT_TRACK)
            const uint64_t mark = wtime::ticks();
#endif
            {
                assert( (size<<1) ==n);
                size_t mmax = 2;
                size_t smax = 1; assert((1<<smax)==mmax);
                while (n>mmax)
                {
                    //__________________________________________________________
                    //
                    // theta=(2*pi)/mmax = 2*pi/(2^smax) = pi/(2^(smax-1))
                    // 0.5*theta=pi/mmax =   pi/(2^smax)
                    //__________________________________________________________
                    
                    //std::cerr << "mmax@" <<  size << ": " << mmax << " 2^" << smax << std::endl;
                    const size_t istep = mmax << 1;
                   // const double theta = isign*(6.28318530717959/mmax);
                    double wtemp = sine[smax]; //sin(0.5*theta);
                    double wpr   =  -2.0*wtemp*wtemp;
                    double wpi   = sine[smax-1];
                    double wr    = 1.0;
                    double wi    = 0.0;
                    for(size_t m=1;m<mmax;m+=2)
                    {
                        for(size_t i=m;i<=n;i+=istep)
                        {
                            T *          data_i = data+i;
                            const size_t j=i+mmax;
                            T *          data_j = data+j;
                            const double djr    = double(data_j[0]);
                            const double dji    = double(data_j[1]);
                            const T      tempr  = T(wr*djr-wi*dji);
                            const T      tempi  = T(wr*dji+wi*djr);
                            data_j[0]  = data_i[0]-tempr;
                            data_j[1]  = data_i[1]-tempi;
                            data_i[0]  += tempr;
                            data_i[1]  += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    ++smax;assert(1<<smax==mmax);
                }
            }
#if defined(YACK_FFT_TRACK)
            algo_ticks += wtime::ticks() - mark;
#endif
        }
        
    
    };

}

#endif

