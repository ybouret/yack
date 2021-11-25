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
        Func<T>    F = { 0 };
        triplet<T> x = { -5, 0, 1 };
        triplet<T> f = { F(x.a), 0, F(x.c) };

        bracket::inside(x,f,F);
        
    }

}

YACK_UTEST(minimize)
{
    test_min<float>();

}
YACK_UDONE()
