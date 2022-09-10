#include "yack/math/multiplier.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void testMult(randomized::bits &ran)
    {
        std::cerr << "min_exp: " << math::multiplier<T>::min_exp << std::endl;
        std::cerr << "max_exp: " << math::multiplier<T>::max_exp << std::endl;

        math::multiplier<T> mm;
        mm.set1();
        T                   raw = 1;
        for(size_t i=1+ran.leq(20);i>0;--i)
        {
            const T ex  = T(20) * ran.symm<T>();
            const T tmp = ran.symm<T>() * std::pow( T(10), ex );
            mm  *= tmp;
            raw *= tmp;
        }
        std::cerr << "mm=" << mm << std::endl;
        std::cerr << "raw=" << raw << std::endl;
        const T prd = mm.query();
        std::cerr << "prd=" << prd << std::endl;

        std::cerr << std::endl;
    }

}

YACK_UTEST(multiplier)
{
    randomized::rand_ ran;
    testMult<float>(ran);
    testMult<double>(ran);
    testMult<long double>(ran);
}
YACK_UDONE()

