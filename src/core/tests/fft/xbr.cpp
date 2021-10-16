
#include "yack/fft/fft1d.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/wtime.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline
    void test_xbr()
    {
        
        for(size_t size=1;size<=8;size<<=1)
        {
            T *data = new T[2*size];
            yack_xbitrev(data-1,size);
            
            delete []data;
        }
        
    }
}

YACK_UTEST(fft_xbr)
{
    uprng ran;
    test_xbr<uint8_t>();
}
YACK_UDONE()

