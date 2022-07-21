
#include "yack/net/types.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/fmt/hexa.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void test_swp( randomized::bits &ran )
    {
        for(size_t i=0;i<10;++i)
        {
            T       x; ran.fill(&x,sizeof(x));
            const T y = endian::swap_be(x);
            const T z = endian::swap_be(y);
            std::cerr << "x=" << x << " | y=" << y << " | z=" <<y <<std::endl;
            YACK_ASSERT(z==x);
        }
    }
}

YACK_UTEST(types)
{
    randomized::rand_ ran;

    std::cerr << std::hex;
    test_swp<uint8_t>(ran);
    test_swp<uint16_t>(ran);
    test_swp<uint32_t>(ran);
    test_swp<uint64_t>(ran);
    test_swp<uint128_t>(ran);

}
YACK_UDONE()

