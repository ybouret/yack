
#include "yack/arith/round10.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

template <unsigned n>
double rounded(const double r)
{
    const double p10 = floor(log10(r))-n;
    return floor(r*pow(10,-p10))*pow(10,p10);
}


YACK_UTEST(slowing_down)
{
    randomized::rand_ ran;

    for(size_t i=0;i<10;++i)
    {
        const double r = ran.to<double>()*pow(10,5-10*ran.to<double>());
        std::cerr << "r   = " << r << std::endl;
        std::cerr << " r0 = " << math::round10<double>::floor_with<0>(r) << std::endl;
        std::cerr << " r1 = " << math::round10<double>::floor_with<0>(r) << std::endl;
        std::cerr << " r2 = " << math::round10<double>::floor_with<0>(r) << std::endl;

    }

}
YACK_UDONE()



