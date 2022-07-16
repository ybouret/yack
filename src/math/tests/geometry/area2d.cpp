#include "yack/math/geometry/area2d.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/triplet.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline void test_triangle()
    {
        const triplet<T> x = {0,3,0};
        const triplet<T> y = {0,0,4};
        const T          S = area2d::triangle(x,y);
        std::cerr << "S=" << S << std::endl;
    }
}

YACK_UTEST(area2d)
{
    test_triangle<float>();
}
YACK_UDONE()

