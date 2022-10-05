#include "yack/arith/round10.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace {

    template <typename T>
    static inline void test_rounding(randomized::bits &ran)
    {
        static const T ten(10);

        for(size_t i=1;i<=10;++i)
        {
            const T r = std::pow(ten,ten*ran.symm<T>());
            std::cerr << std::setw(15) << r;

            std::cerr << " -> " << std::setw(15) << math::round10<T>::template floor_with<0>(r);
            std::cerr << " -> " << std::setw(15) << math::round10<T>::template floor_with<1>(r);
            std::cerr << " -> " << std::setw(15) << math::round10<T>::template floor_with<2>(r);

            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}

YACK_UTEST(round10)
{
    randomized::rand_ ran;

    test_rounding<float>(ran);
    test_rounding<double>(ran);
    test_rounding<long double>(ran);

}
YACK_UDONE()

