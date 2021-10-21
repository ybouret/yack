

#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    template <typename T> static inline
    void test_base2()
    {
        std::cerr << std::endl;
        T p=0;
        while(p<base2<T>::max_power_of_two)
        {
            p = next_power_of_two(++p);
            std::cerr << uint64_t(p) << std::endl;
            YACK_ASSERT(is_a_power_of_two(p));
            const unsigned l = integer_log2(p);
            YACK_ASSERT( (T(1) << l) == p );
        }

    }

    template <typename T> static inline
    void test_bytes_for(randomized::bits &ran)
    {
        std::cerr << std::hex;
        T p = 0;
        YACK_ASSERT(0==bytes_for(p));
        for(size_t i=sizeof(T)*8;i>0;--i)
        {
            p <<= 1;
            p |= ran() > 0.5 ? 1 : 0;
            const size_t B = bytes_for(p);
            std::cerr << uint64_t(p) << " => byte=" << B << std::endl;
        }

    }


}


YACK_UTEST(base2)
{
    test_base2<int8_t>();
    test_base2<uint8_t>();

    test_base2<int16_t>();
    test_base2<uint16_t>();

    test_base2<int32_t>();
    test_base2<uint32_t>();

    test_base2<int64_t>();
    test_base2<uint64_t>();

    randomized::rand_ ran( time(NULL) );
    test_bytes_for<uint8_t>(ran);
    test_bytes_for<uint16_t>(ran);
    test_bytes_for<uint32_t>(ran);
    test_bytes_for<uint64_t>(ran);


}
YACK_UDONE()

