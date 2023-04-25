#include "yack/math/algebra/lu-kernel.hpp"
#include "yack/utest/run.hpp"


using namespace yack;
using namespace math;

YACK_UTEST(lu)
{
    lu_kernel luk(10,32,64);
    std::cerr << "granted: " << luk.granted() << std::endl;
}
YACK_UDONE()

