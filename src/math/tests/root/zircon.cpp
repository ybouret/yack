#include "yack/math/root/zircon.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

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
        std::cerr << "zircon<" << rtti::name<T>() << ">" << std::endl;

        F2D<T>    F = { 0 };
        zircon<T> solver(2);

        solver.X[1] = 0.1;  solver.X[2] = 0.2;
        solver.X[1] = 1e-4; solver.X[2] = 2e-3;

        switch( solver.load(F) )
        {
            case core::zircon::singular:
                std::cerr << "singular" << std::endl; break;
            case core::zircon::regular:
                std::cerr << "regular" << std::endl;
                solver.forward(F);
                break;
            case core::zircon::degenerate:
                std::cerr << "degenerate" << std::endl;
                solver.forward(F);
                break;
        }

        std::cerr << std::endl;
    }

}

YACK_UTEST(zircon)
{
    do_zircon<float>();
    do_zircon<double>();
    //do_zircon<long double>();

    YACK_SIZEOF(zircon<float>);
    YACK_SIZEOF(zircon<double>);
    YACK_SIZEOF(zircon<long double>);

}
YACK_UDONE()

