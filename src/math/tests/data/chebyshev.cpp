
#include "yack/math/data/chebyshev.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string/cxx-name.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace math;

namespace
{

    template <typename T>
    static inline T F(T x)
    {
        return T(0.1) + sin( T(1.3)*x - T(0.2));
    }

    template <typename T>
    static inline void test_cheb( randomized::bits &ran )
    {
        const string fn = "cheb-" + cxx_name::of( rtti::name<T>() ) + ".dat";
        std::cerr << fn << std::endl;

        chebyshev<T> cheb;
        const T      a=-2;
        const T      b=3;
        cheb.build(F<T>,a,b,8);

        vector<T>    x;
        {
            const size_t N = 10 + ran.leq(10);
            x.push_back(a);
            for(size_t i=1;i<N;++i)
            {
                x.push_back(a + i*(b-a)/N );
            }
            x.push_back(b);
        }

        ios::ocstream::overwrite(fn);
        for(size_t i=1;i<=x.size();++i)
        {
            ios::ocstream::echo(fn,"%g %g", double(x[i]), double(F<T>(x[i])));
            for(size_t n=0;n<=cheb.mutual_size();++n)
            {
                ios::ocstream::echo(fn," %g", double( cheb.at(x[i],a,b,n)));
            }
            ios::ocstream::echo(fn,"\n");
        }





    }

}

YACK_UTEST(chebyshev)
{
    randomized::rand_ ran;
    test_cheb<float>(ran);
}
YACK_UDONE()
