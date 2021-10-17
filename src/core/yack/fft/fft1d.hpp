
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

namespace yack
{
    //! inline cswap of integral objects
    template <typename T> inline
    void cswap2(T *lhs, T *rhs) throw()
    {
        assert(lhs); assert(rhs);
        const T tmp0 = lhs[0];
        const T tmp1 = lhs[1];
        lhs[0] = rhs[0];
        lhs[1] = rhs[1];
        rhs[0] = tmp0;
        rhs[1] = tmp1;
    }
}

#include "yack/fft/xbr-decl.hxx"


namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! FFT in 1D
    //
    //__________________________________________________________________________
    struct fft1d
    {
        //______________________________________________________________________
        //
        // tracking
        //______________________________________________________________________
        static volatile uint64_t algo_ticks; //!< for time tracking
        
        //______________________________________________________________________
        //
        // parameters
        //______________________________________________________________________
        static const size_t one      = 1;    //!< size_t(1)
        static const size_t max_exp2 = (sizeof(size_t)<<3) - 1; //!< highest bit
        static const size_t max_size = one << max_exp2;         //!< highest size
       
        //______________________________________________________________________
        //
        // precomputed tables
        //______________________________________________________________________
        static const double pos_sine[64]; //!< sin(pi/(2^indx))
        static const double neg_sine[64]; //!< -sin(pi/(2^indx))
        static const double twpr[64];     //!< -2*(sin(pi/(2^indx)))^2
        
        //______________________________________________________________________
        //
        //! forward algorithm data[2*size], mapping complexes
        //______________________________________________________________________
        template <typename T> static
        inline void forward(T data[], const size_t size) throw()
        {
            apply(data,size,pos_sine);
        }
        
        //______________________________________________________________________
        //
        //! reverse algorithm data[2*size], mapping complexes
        //______________________________________________________________________
        template <typename T> static
        inline void reverse(T data[], const size_t size) throw()
        {
            apply(data,size,neg_sine);
        }
        
        //______________________________________________________________________
        //
        //! default bit reversal routine
        //______________________________________________________________________
        template <typename T> static
        inline void bitrev(T data[],const size_t size) throw()
        {
            assert(data);
            assert(size);
            const size_t n = (size << 1);
            for(size_t i=1,j=1;i<n;i+=2)
            {
                if(j>i)
                {
                    cswap2(data+i,data+j);
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
        
    private:
        template <typename T> static
        inline void apply(T            *data,
                          const size_t  size,
                          const double *sine) throw()
        {
            
            xbitrev(data,size);

            {
                const size_t n = (size << 1);
#if             defined(YACK_FFT_TRACK)
                const uint64_t mark = wtime::ticks();
#endif
                assert( (size<<1) ==n);
                size_t mmax = 2;
                size_t smax = 1; //assert((1<<smax)==mmax);
                while (n>mmax)
                {
                    //__________________________________________________________
                    //
                    // theta    = (2*pi)/mmax = 2*pi/(2^smax) = pi/(2^(smax-1))
                    // 0.5*theta= pi/mmax =   pi/(2^smax)
                    //__________________________________________________________
                    
                    const size_t istep = mmax << 1;
                    double wtemp = sine[smax];   //sin(0.5*theta);
                    double wpr   = twpr[smax];   //-2.0*wtemp*wtemp;
                    double wpi   = sine[smax-1]; // sin(theta)
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
#if             defined(YACK_FFT_TRACK)
                algo_ticks += wtime::ticks() - mark;
#endif
            }

        }
        
    
    };

}

#endif

