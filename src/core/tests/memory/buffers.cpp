#include "yack/memory/buffers.hpp"
#include "yack/utest/run.hpp"

#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/allocator/pooled.hpp"

using namespace yack;

namespace
{



    template <typename T>
    static inline void test_buff( const size_t n, randomized::bits &ran )
    {
        std::cerr << "buffer<sizeof=" << sizeof(T) << ">(" << n << ")" << std::endl;

        memory::buffer_of<T,memory::global> gbuf(n);
        memory::buffer_of<T,memory::dyadic> dbuf(n);
        memory::buffer_of<T,memory::pooled> pbuf(n);

        YACK_CHECK(gbuf.size()>=n);
        YACK_CHECK(dbuf.size()>=n);
        YACK_CHECK(pbuf.size()>=n);

        for(size_t i=n;i>0;--i)
        {
            gbuf[i] = ran.full<T>();
            dbuf[i] = ran.full<T>();
            pbuf[i] = ran.full<T>();
        }


    }

    template <typename T>
    static inline void test_buff( randomized::bits &ran )
    {

        test_buff<T>(0,ran);
        test_buff<T>(1,ran);
        test_buff<T>(3,ran);
        test_buff<T>(10,ran);
        test_buff<T>(100,ran);


    }

}


YACK_UTEST(memory_buffers)
{
    randomized::rand_ ran;

    test_buff<int8_t>(ran);
    test_buff<int16_t>(ran);
    test_buff<int32_t>(ran);
    test_buff<int64_t>(ran);


}
YACK_UDONE()

