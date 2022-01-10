#include "yack/math/triplet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(triplet)
{
    randomized::rand_ ran;
    triplet<float> X = { 3, 1, 2 };

    std::cerr << X << std::endl;
    X.sort();
    std::cerr << X << std::endl;
    YACK_CHECK(X.is_increasing());

    randomized::shuffle::data(&X[1],3,ran);
    triplet<double> Y = { 4, 5, 6 };
    std::cerr << "X=" << X << std::endl;
    std::cerr << "Y=" << Y << std::endl;
    X.sort(Y);
    YACK_CHECK(X.is_increasing());
    std::cerr << "X=" << X << std::endl;
    std::cerr << "Y=" << Y << std::endl;



}
YACK_UDONE()

