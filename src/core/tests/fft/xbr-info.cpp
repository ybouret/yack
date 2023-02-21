

#include "yack/fft/fft1d.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/wtime.hpp"
#include "yack/arith/base2.hpp"

using namespace yack;

namespace
{
    static inline
    size_t  count_xbitrev(const size_t size, size_t &jmax) noexcept
    {
        assert(size);
        size_t       count = 0;
        const size_t n     = (size << 1);
        for(size_t i=1,j=1;i<n;i+=2)
        {
            if(j>i)
            {
                jmax = max_of(j,jmax);
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
    
    static inline void emit_xbitrev(const size_t size) noexcept
    {
        const size_t n     = (size << 1);
        size_t       jmax  = 0;
        const size_t count = count_xbitrev(size,jmax);
        size_t       k     = 0;
        for(size_t i=1,j=1;i<n;i+=2)
        {
            if(j>i)
            {
                ++k;
                std::cerr << "{" << i << "," << j << "}";
                if(k<count)  std::cerr << ',';
                if(0==(k&7)) std::cerr << std::endl;
            }
            size_t m=size;
            while( (m>=2) && (j>m) )
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        std::cerr << std::endl;
    }
    
}

YACK_UTEST(fft_xbr_info)
{
    for(size_t size=1;size<=(1<<16);size<<=1)
    {
        size_t         jmax  = 0;
        const size_t   count = count_xbitrev(size,jmax);
        std::cerr << std::setw(8)     << size << " => ";
        std::cerr << std::setw(8)     << count;
        std::cerr << " jmax=" << std::setw(8) << jmax;
        if(jmax>0)
        {
            const size_t  bmax  = next_power_of_two( bytes_for( uint64_t(jmax) ) );
            const size_t  bits  = bmax << 3;
            const size_t  bytes = bmax * count; // for I or J
            std::cerr << " -> uint" << bits << "_t";
            std::cerr << " -> bytes: " << bytes*2;
        }
        std::cerr << std::endl;
        if(size<=8)
            emit_xbitrev(size);
     }
}
YACK_UDONE()

