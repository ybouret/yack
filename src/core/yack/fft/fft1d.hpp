
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

        //______________________________________________________________________
        //
        //! expand fft1 and fft2 from a dual-real-functions transform
        //______________________________________________________________________
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
                const T      rep=half*(A+B);
                const T      rem=half*(A-B);
                const T      aip=half*(C+D);
                const T      aim=half*(C-D);

                fft1[j]  =  rep;
                fft1[j1] =  aim;
                fft1[j2] =  rep;
                fft1[j3] = -aim;

                fft2[j]  =  aip;
                fft2[j1] = -rem;
                fft2[j2] =  aip;
                fft2[j3] =  rem;
            }
        }
        
    private:
        template <typename T> static
        inline void apply(T            *data,
                          const size_t  size,
                          const double *sine) throw()
        {

            //__________________________________________________________________
            //
            // bit reversal
            //__________________________________________________________________
            xbitrev(data,size);

#if         defined(YACK_FFT_TRACK)
            const uint64_t mark = wtime::ticks();
#endif
            //__________________________________________________________________
            //
            // algorithm
            //__________________________________________________________________
            const size_t   n    = (size << 1);
            const double *temp = twpr;
            size_t        mmax = 2;
            while(n>mmax)
            {

                const size_t istep = mmax << 1;
                double wpi         = *(sine++);   // sin(theta)
                double wpr         = *(++temp);   //-2.0*wtemp*wtemp;
                double wr          = 1.0;
                double wi          = 0.0;
                for(size_t m=1;m<mmax;m+=2)
                {
                    //__________________________________________________________
                    //
                    // data modification
                    //__________________________________________________________
                    const double sw     = wr+wi;
                    for(size_t i=m;i<=n;i+=istep)
                    {
                        T *          data_i = data+i;
                        T *          data_j = data_i + mmax;
                        const double dr     = double(data_j[0]);
                        const double di     = double(data_j[1]);
                        const double rere   = wr*dr;
                        const double imim   = wi*di;
                        const double sd     = dr+di;
                        const T      tempr  = T(rere-imim);
                        const T      tempi  = T(sw*sd-rere-imim);
                        data_j[0]  =  data_i[0]-tempr;
                        data_j[1]  =  data_i[1]-tempi;
                        data_i[0]  += tempr;
                        data_i[1]  += tempi;
                    }

                    //__________________________________________________________
                    //
                    // update factors
                    //__________________________________________________________
                    {
                        const double rere   = wr*wpr;
                        const double imim   = wi*wpi;
                        const double sp     = wpr+wpi;
                        wr += (rere-imim);
                        wi += (sw*sp-rere-imim);
                    }

                }
                mmax=istep;
            }
#if         defined(YACK_FFT_TRACK)
            algo_ticks += wtime::ticks() - mark;
#endif
            
        }
        
        
    };
    
}

#endif

