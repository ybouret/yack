
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
    namespace fft
    {

        template <const size_t exp2>
        struct bitrev
        {
            //! data[1..2*(size=2^exp2)], for complex<T>
            template <typename T> static inline
            void run(T data[]) throw()
            {
                static const size_t size = size_t(1) << exp2;
                static const size_t n    = size << 1;
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
        };


        template <> struct bitrev<0>
        {
            template <typename T>
            static inline void run(T[]) throw() {}
        };

        template <> struct bitrev<1>
        {
            template <typename T>
            static inline void run(T[]) throw() {}
        };



        //!  count number of exchanges
        template < const size_t exp2>
        inline size_t bitrev_count() throw()
        {
            static const size_t size = size_t(1) << exp2;
            static const size_t n    = size << 1;
            size_t count = 0;
            for(size_t i=1,j=1;i<n;i+=2)
            {
                if(j>i)
                {
                    ++count;
                }
                size_t m=size;
                while( (m>=2) && (j>m) )
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }
            return count;
        }

        
        template <typename T,const size_t exp2>
        inline void apply(T data[], const int isign)
        {
            bitrev<exp2>::run(data);
            
            static const size_t size = size_t(1) << exp2;
            size_t n    = size << 1;
            size_t mmax = 2;
            size_t smax = 1; assert(1<<smax==mmax);
            while (n>mmax)
            {
                //std::cerr << "mmax@" <<  size << ": " << mmax << std::endl;
                const size_t istep = mmax << 1;
                const double theta = isign*(6.28318530717959/mmax);
                double wtemp = sin(0.5*theta);
                double wpr   = -2.0*wtemp*wtemp;
                double wpi   = sin(theta);
                double wr = 1.0;
                double wi = 0.0;
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


}


#endif
