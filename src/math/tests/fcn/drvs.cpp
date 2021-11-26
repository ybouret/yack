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
        T  operator()(T x)
        {
            return cos(x);
        }

    };

    template <typename T>
    static inline  void test()
    {
        std::cerr << "drvs for " << rtti::name<T>() << std::endl;
        std::cerr << "   unit_step_size: " << derivative<T>::unit_step_size() << std::endl;
        std::cerr << "   diff_maxi_ftol: " << derivative<T>::diff_maxi_ftol() << std::endl;

        Func<T>       F = { 0 };
        derivative<T> drvs;
        const T       h(1e-4);
        const string  filename = vformat("drvs%u.dat", unsigned(sizeof(T)*8) );
        ios::ocstream fp(filename);
        for(T x=-10;x<=10;x+=T(0.01))
        {
            const T dF  = drvs.diff(F,x,h);
            const T f0  = F(x);
            fp("%Lg %Lg %Lg\n", (long double)x, (long double)f0, (long double)dF);
        }

    }

}

YACK_UTEST(drvs)
{
    test<float>();
    test<double>();
    test<long double>();

}
YACK_UDONE()


