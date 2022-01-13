
#include "yack/math/fit/least-squares.hpp"
#include "yack/math/fit/real-samples.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace {


}


YACK_UTEST(gls)
{

    fit::real_samples_of<double> both("both");

    fit::real_sample_of<double> &s1 = both("s1");
    fit::real_sample_of<double> &s2 = both("s2");
    



}
YACK_UDONE()

