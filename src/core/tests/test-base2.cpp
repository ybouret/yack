

#include "yack/arith/base2.h"
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
}


YACK_UTEST(base2)
{
    {
        std::cerr << "yack_maxi_po2=" << yack_maxi_power_of_two << std::endl;
        size_t p = 0;
        while(p<yack_maxi_power_of_two)
        {
            p = yack_next_power_of_two(++p);
            std::cerr << p << std::endl;
            YACK_ASSERT(yack_is_a_power_of_two(p));
        }
    }

    test_base2<int8_t>();
    test_base2<uint8_t>();

    test_base2<int16_t>();
    test_base2<uint16_t>();

    test_base2<int32_t>();
    test_base2<uint32_t>();

    test_base2<int64_t>();
    test_base2<uint64_t>();

}
YACK_UDONE()

