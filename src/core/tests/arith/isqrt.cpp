#include "yack/arith/isqrt.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/ints.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "yack/arith/base2.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void test_isqrt(const bool show = true)
    {
        std::cerr << "test " << rtti::name<T>() << std::endl;
        for(uint64_t i=0;i<=integral_for<T>::maximum;++i)
        {
            const T x = static_cast<T>(i);
            const T s = isqrt(x);
            if(show)
                std::cerr << " / " << uint64_t(x) << " = " << uint64_t(s);
            YACK_ASSERT(s*s<=x);
        }
        std::cerr << std::endl;
    }
}

YACK_UTEST(arith_isqrt)
{
    test_isqrt<uint8_t>();
    test_isqrt<uint16_t>();
    //test_isqrt<uint32_t>(false);
    {
        const uint32_t xmax = integral_for<uint32_t>::maximum;
        const uint32_t smax = isqrt(xmax);
        std::cerr << xmax << " => " << smax << std::endl; YACK_ASSERT(smax*smax<=xmax);
    }


    {
        const uint64_t xmax = integral_for<uint64_t>::maximum;
        const uint64_t smax = isqrt(xmax);
        std::cerr << xmax << " => " << smax << std::endl; YACK_ASSERT(smax*smax<=xmax);
    }
    
    for(uint64_t x=2; x<base2<uint32_t>::max_power_of_two;x *= x)
    {
        const uint64_t s = isqrt(x);
        std::cerr << x << " => " << s << std::endl;
    }



}
YACK_UDONE()

