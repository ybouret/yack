#include "yack/math/opt/minimize.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

namespace
{
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


    template <typename T>
    static inline void test_min(const T x1, const T x2)
    {
        Func<T>    F = { 0 };
        triplet<T> x = { x1,     0,   x2   };
        triplet<T> f = { F(x.a), 0, F(x.c) };

        if( bracket::inside(x,f,F) )
        {

            x.sort(f);
            T w     = fabs(x.c-x.a);
            for(size_t iter=1;iter<=5;++iter)
            {
                T new_w = minimize::move<T>::run_(x,f,F);
                std::cerr << "w=" << w << " -> " << new_w << std::endl;
                std::cerr << "x=" << x << ", f=" << f << std::endl;
                std::cerr << "dw=" <<  w-new_w << std::endl;
                w = new_w;
                std::cerr << "minimax: " << f.minimax() << std::endl;
            }




        }
        else
        {
            std::cerr << "not bracketed" << std::endl;
        }

    }

    template <typename T>
    static inline void test_min()
    {
        std::cerr << "minimize<" << rtti::name<T>() << ">" << std::endl;
        test_min<T>(-5,2);
        test_min<T>(0,10);
        std::cerr << std::endl;
    }

}

YACK_UTEST(minimize)
{

    //test_min<float>();

    test_min<double>();
    //test_min<long double>();

}
YACK_UDONE()
