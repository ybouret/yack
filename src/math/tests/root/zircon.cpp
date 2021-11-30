#include "yack/math/root/zircon.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace {

    template <typename T>
    struct F2D
    {

        T y0;

        inline void operator()(writable<T> &F, const readable<T> &X)
        {
            const T x = X[1];
            const T y = X[2];
            F[1] = x*x+y*y - 1;
            F[2] = y-(y0+x);
        }

    };


    template <typename T>
    static inline void do_zircon()
    {
        F2D<T> F = { 0 };
        zircon<T> solver;

        solver.X[1] = 0.1; solver.X[2] = 0.2;
        solver.load(F);
        
    }

}

YACK_UTEST(zircon)
{
    do_zircon<float>();
    do_zircon<double>();
    do_zircon<long double>();

    YACK_SIZEOF(zircon<float>);
    YACK_SIZEOF(zircon<double>);
    YACK_SIZEOF(zircon<long double>);

}
YACK_UDONE()

