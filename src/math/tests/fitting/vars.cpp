
#include "yack/math/fitting/replica.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{

}


YACK_UTEST(fitting_vars)
{
    fitting::primary::handle    v = new fitting::primary("hello",1);
    fitting::variable::pointer  w = new fitting::replica("world",v);
    std::cerr << v << std::endl;
    std::cerr << w << std::endl;

}
YACK_UDONE()
