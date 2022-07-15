
#include "yack/net/types.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/fmt/hexa.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void test_swp( randomized::bits &ran )
    {
        T x = 0; ran.fill(&x,sizeof(x));
        const T y = nbo::swap(x);
        std::cerr << "x=" << ios::hexa(x) << " | y=" << y << std::endl;

    }
}

YACK_UTEST(types)
{
    randomized::rand_ ran;

    test_swp<uint8_t>(ran);
    test_swp<uint16_t>(ran);
    test_swp<uint32_t>(ran);


}
YACK_UDONE()

