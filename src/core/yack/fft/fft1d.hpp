
//! \file

#ifndef YACK_FFT1D_INCLUDED
#define YACK_FFT1D_INCLUDED 1

#include "yack/fft/xbitrev.hpp"

#define  YACK_FFT_TRACK
#if defined(YACK_FFT_TRACK)
#include "yack/system/wtime.hpp"
#endif


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
        
        template <typename T> static
        inline void expand(T fft1[], T fft2[], const size_t n) throw()
        {
            static const T half(0.5);
            const size_t nn2 = 2+n+n;
            const size_t nn3 = 1+nn2;
            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=0;
            for(size_t j=n+1;j>=3;j-=2)
            {
                const size_t j1 = j+1;
                const size_t j2 = nn2-j;
                const size_t j3 = nn3-j;
                const T      A  = fft1[j];
                const T      B  = fft1[j2];
                const T      C  = fft1[j1];
                const T      D  = fft1[j3];
                const T rep=half*(A+B);
                const T rem=half*(A-B);
                const T aip=half*(C+D);
                const T aim=half*(C-D);
                fft1[j]  =  rep;
                fft1[j1] =  aim;
                fft1[j2] =  rep;
                fft1[j3] = -aim;
                fft2[j]  =  aip;
                fft2[j1] = -rem;
                fft2[j2] = aip;
                fft2[j3] = rem;
            }
        }
        
        
    private:
        template <typename T> static
        inline void apply(T            *data,
                          const size_t  size,
                          const double *sine) throw()
        {
            
            xbitrev(data,size);
            
            const size_t   n    = (size << 1);
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
                        T *          data_j = data_i + mmax;
                        const double djr    = double(data_j[0]);
                        const double dji    = double(data_j[1]);
                        // TODO: speed up complex mul
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
                ++smax;
            }
#if             defined(YACK_FFT_TRACK)
            algo_ticks += wtime::ticks() - mark;
#endif
            
        }
        
        
    };
    
}

#endif

