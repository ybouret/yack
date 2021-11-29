#include "yack/math/opt/bracket.hpp"
#include "yack/utest/run.hpp"

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
            return 0.1 - cos(0.2*x-0.3);
        }
    };

    template <typename T>
    static inline void test_min()
    {
        {
            Func<T>    F = { 0 };
            triplet<T> x = { 1, 0, -5 };
            triplet<T> f = { F(x.a), 0, F(x.c) };

            if( bracket::inside(x,f,F) )
            {
                std::cerr << "bracketed" << std::endl;
                std::cerr << "x=" << x << std::endl;
                std::cerr << "f=" << f << std::endl;
            }
            else
            {
                std::cerr << "not bracketed" << std::endl;
            }

        }

        

        
    }

}

YACK_UTEST(minimize)
{
    test_min<float>();
    test_min<double>();
    test_min<long double>();

}
YACK_UDONE()
