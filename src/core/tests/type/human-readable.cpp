

#include "yack/type/human-readable.hpp"
#include "yack/type/ints.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(type_human)
{

    const uint64_t umax = integral_for<uint64_t>::maximum;
    //const double   dmax = umax;
    std::cerr << "umax=" << umax << std::endl;
    //std::cerr << "dmax=" << dmax << std::endl;

    const uint64_t u1 = 1;

    std::cerr << "k => " << (u1<<10) << std::endl;
    std::cerr << "M => " << (u1<<20) << std::endl;
    std::cerr << "G => " << (u1<<30) << std::endl;
    std::cerr << "T => " << (u1<<40) << std::endl;
    std::cerr << "P => " << (u1<<50) << std::endl;
    std::cerr << "E => " << (u1<<60) << std::endl;

    {human_readable h(u1);}

    {human_readable h(umax);}

    { human_readable h(1000000); }
    { human_readable h(1000000000); }
    { human_readable h(1000000000000); }


}
YACK_UDONE()
