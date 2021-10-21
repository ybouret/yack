
#include "yack/fft/xbitrev.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/wtime.hpp"
#include "yack/randomized/rand.hpp"
#include <cstring>

using namespace yack;

namespace yack
{
    //__________________________________________________________________________
    //
    //! default bit reversal routine
    //__________________________________________________________________________
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
}

namespace
{
    static double tsample=0.2;

    template <typename T>
    static inline
    void test_xbr(randomized::bits &ran)
    {
        wtime chrono;
        
        for(size_t size=1;size<=65536;size<<=1)
        {
            std::cerr << "size=" << std::setw(6) << size;
            const size_t n = size*2;
            T *data = new T[2*n];
            ran.fill(data,2*n*sizeof(T));
            T *work = data+n;
            for(size_t i=0;i<n;++i) work[i] = data[i];
            YACK_ASSERT(0==memcmp(work,data,n*sizeof(T)));
            yack::xbitrev(data-1,size);
            yack::bitrev(work-1,size);
            YACK_ASSERT(0==memcmp(work,data,n*sizeof(T)));
            
            uint64_t std_ticks = 0;
            uint64_t opt_ticks = 0;
            
            size_t iter=0;
            do
            {
                ++iter;
                {
                    const uint64_t mark = wtime::ticks();
                    yack::bitrev(work-1,size);
                    std_ticks += wtime::ticks() - mark;
                }
                
                {
                    const uint64_t mark = wtime::ticks();
                    yack::xbitrev(work-1,size);
                    opt_ticks += wtime::ticks() - mark;
                }
            } while( chrono(std_ticks)<=tsample );
            
            const double std_rate =1e-6 * iter/chrono(std_ticks);
            const double opt_rate =1e-6 * iter/chrono(opt_ticks);
            const double speed_up = opt_rate/std_rate;
            std::cerr << " | std_rate: " << std::setw(8) << std_rate;
            std::cerr << " | opt_rate: " << std::setw(8) << opt_rate;
            std::cerr << " | speed_up: " << std::setw(8) << speed_up;

            std::cerr << std::endl;
            
            delete []data;
        }
        
    }
}

YACK_UTEST(fft_xbr8)
{
    if(argc>1) tsample = atof(argv[1]);
    randomized::rand_  ran;
    std::cerr.precision(4);
    test_xbr<uint8_t>(ran);
}
YACK_UDONE()

YACK_UTEST(fft_xbr64)
{
    if(argc>1) tsample = atof(argv[1]);
    randomized::rand_  ran;
    std::cerr.precision(4);
    test_xbr<uint64_t>(ran);
    
}
YACK_UDONE()
