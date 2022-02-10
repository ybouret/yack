#include "yack/math/opt/minimize.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace math;

namespace
{
    static float xtol = 1e-6;

    template <typename T>
    struct Func
    {
        size_t count;

        T operator()(T x)
        {
            ++count;
            return 0.1 - cos(0.2*x-0.3);
        }
    };



    template <typename T> static inline
    void test_min(const T x1, const T x2)
    {
        std::cerr << "Minimizing in " << x1 << ":" << x2 << std::endl;
        Func<T>    F = { 0 };
        triplet<T> x = { x1,     0,   x2   };
        triplet<T> f = { F(x.a), 0, F(x.c) };


        const T x_opt = minimize::find<T>::run_for(F,x,f,minimize::inside,T(xtol));

        std::cerr << "\tf_opt=" << f.b << "@" << x_opt << ", #calls=" << F.count << std::endl;

         
        std::cerr << std::endl;

    }

    template <typename T>
    static inline void test_min()
    {
        std::cerr << "minimize<" << rtti::name<T>() << ">" << std::endl;
        test_min<T>(-5,2);
        test_min<T>(0,10);
        test_min<T>(-5,-1);
        std::cerr << std::endl;
    }

}

YACK_UTEST(minimize)
{
    minimize::verbose = true;

    if(argc>1)
    {
        xtol =  ios::ascii::convert::real<float>(argv[1],"xtol");
    }

    test_min<float>();
    test_min<double>();
    test_min<long double>();

}
YACK_UDONE()
