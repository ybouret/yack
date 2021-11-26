#include "yack/math/fcn/derivative.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

namespace
{

    template <typename T>
    struct Func
    {
        size_t count;
        double operator()(T x)
        {
            static const T a(0.2);
            return sin(a*x*x);
        }

    };

    template <typename T>
    static inline  void test()
    {
        std::cerr << "drvs for " << rtti::name<T>() << std::endl;
        Func<T>       F = { 0 };
        derivative<T> drvs;
        T             err = -1;
        const T       h(1e-4);
        const string  filename = vformat("drvs%u.dat", unsigned(sizeof(T)*8) );
        ios::ocstream fp(filename);
        for(T x=-10;x<=10;x+=T(0.1))
        {
            const T dF = drvs(F,x,h,err);
            fp("%llg %llg %llg\n", x, F(x), dF);
        }

    }

}

YACK_UTEST(drvs)
{
    test<float>();
}
YACK_UDONE()


