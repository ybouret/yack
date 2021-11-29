#include "yack/math/data/polynomial.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

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

        {
            const string  filename = vformat("polint-tgt%u.dat", bits );
            ios::ocstream fp(filename);

            polynomial::interpolate<T> interp;
            const T xmin = -T(0.1);
            const T xmax = xx - xmin;
            for(T x=xmin; x <= xmax; x += T(0.01) )
            {
                T dy = 0;
                T y  = interp(x,xa,ya,dy);
                fp("%Lg %Lg %Lg\n", (long double) x, (long double) y, (long double)dy );

            }
        }


    }

}

YACK_UTEST(poly)
{
    randomized::rand_ ran;

    test_polint<float>(ran);
    test_polint<double>(ran);
    test_polint<long double>(ran);

}
YACK_UDONE()


