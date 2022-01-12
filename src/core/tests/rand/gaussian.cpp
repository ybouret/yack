

#include "yack/randomized/mt19937.hpp"

#include "yack/randomized/gaussian.hpp"

#include "yack/utest/run.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include <cstring>
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(rand_gaussian)
{
    randomized::shared_bits      mt  = new randomized::mt19937();
    randomized::gaussian<double> gran(mt);

    double sum  = 0;
    double sum2 = 0;

    unsigned count = 0;
    ios::ocstream fp("gran.dat");
    for(size_t i=0;i<100000;++i)
    {
        const double g = gran();
        sum  += g;
        sum2 += g*g;
        ++count;
        fp("%u %g %g\n", count, sum/count, sum2/count);
    }







}
YACK_UDONE()


