#include "yack/math/triplet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(triplet)
{
    triplet<float> X = { 3, 1, 2 };

    std::cerr << X << std::endl;
    X.sort();
    std::cerr << X << std::endl;
    YACK_CHECK(X.is_increasing());

}
YACK_UDONE()

