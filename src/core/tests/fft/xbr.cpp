
#include "yack/fft/fft1d.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/wtime.hpp"
#include <cstring>

using namespace yack;

namespace
{
    template <typename T>
    static inline
    void test_xbr(uprng &ran)
    {
        wtime chrono;
        
        for(size_t size=1;size<=2048;size<<=1)
        {
            std::cerr << "size=" << std::setw(6) << size;
            const size_t n = size*2;
            T *data = new T[2*n];
            ran.fillnz(data,n*sizeof(data));
            T *work = data+n;
            for(size_t i=0;i<n;++i) work[i] = data[i];
            YACK_ASSERT(0==memcmp(work,data,n*sizeof(T)));
            yack_xbitrev(data-1,size);
            fft1d::bitrev(work-1,size);
            YACK_ASSERT(0==memcmp(work,data,n*sizeof(T)));
            
            uint64_t std_ticks = 0;
            uint64_t opt_ticks = 0;
            
            size_t iter=0;
            do
            {
                ++iter;
                {
                    const uint64_t mark = wtime::ticks();
                    fft1d::bitrev(work-1,size);
                    std_ticks += wtime::ticks() - mark;
                }
                
                {
                    const uint64_t mark = wtime::ticks();
                    yack_xbitrev(work-1,size);
                    opt_ticks += wtime::ticks() - mark;
                }
            } while( chrono(opt_ticks)<=0.25 );
            
            const double std_rate =1e-6 * iter/chrono(std_ticks);
            const double opt_rate =1e-6 * iter/chrono(opt_ticks);
            std::cerr << " | std_rate: " << std_rate;
            std::cerr << " | opt_rate: " << opt_rate;
            std::cerr << std::endl;
            
            delete []data;
        }
        
    }
}

YACK_UTEST(fft_xbr)
{
    uprng ran;
    std::cerr.precision(4);
    test_xbr<uint8_t>(ran);
}
YACK_UDONE()

