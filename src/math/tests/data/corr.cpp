
#include "yack/math/data/corr.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline void make_corr( randomized::bits &ran )
    {

        correlation<T> corr;

        for(size_t iter=0;iter<2;++iter)
        {
            corr.free();
            YACK_CHECK(0==corr.size());
            const size_t n = 10 + ran.leq(30);
            const T a = ran.symm<T>() * 10;
            const T b = ran.symm<T>() * 10;
            for(size_t i=n;i>0;--i)
            {
                const T x = ran.symm<T>()* 30;
                const T y = a*x+b;
                corr(y,y+ran.symm<T>() * 0.5);
            }
            YACK_CHECK(n==corr.size());
            std::cerr << "corr=" << corr() << " //" << corr.size() << std::endl;
        }

        std::cerr << std::endl;

    }
}

YACK_UTEST(corr)
{
    randomized::rand_ ran;

    make_corr<float>(ran);
    make_corr<double>(ran);
    make_corr<long double>(ran);



}
YACK_UDONE()
