#include "yack/math/data/linear.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(linear)
{
    double x[4] = {0,1.1,2.2,3.3};
    double y[4] = {0,-1,2,-3};
    
    const thin_array<double> X(x,4);
    const thin_array<double> Y(y,4);
    
    std::cerr << linear(0.5,X,Y) << std::endl;
    
    
}
YACK_UDONE()

