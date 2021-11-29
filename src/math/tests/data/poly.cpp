#include "yack/math/data/polynomial.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace
{

    template <typename T> static inline
    void test_polint(randomized::bits &ran)
    {
        const unsigned bits = sizeof(T) * 8;
        const size_t   n    = 5 + ran.leq(10);
        vector<T>    xa(n,as_capacity);
        vector<T>    ya(n,as_capacity);
        T xx = 0;
        {
            const string filename = vformat("polint-src%u.dat", bits );
            ios::ocstream fp(filename);
            for(size_t i=n;i>0;--i)
            {
                xa.push_back( xx += ran.to<T>() );
                ya.push_back( sin(xa.back()) );
                fp("%Lg %Lg\n", (long double) xa.back(), (long double) ya.back() );
            }
        }
        


    }

}

YACK_UTEST(poly)
{
    randomized::rand_ ran;

    test_polint<float>(ran);
    test_polint<long double>(ran);

}
YACK_UDONE()


