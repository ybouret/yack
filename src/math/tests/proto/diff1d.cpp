#include "yack/utest/run.hpp"
#include "yack/randomized/mt19937.hpp"
#include "yack/randomized/gaussian.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/sequence/stats.hpp"

using namespace yack;
//using namespace math;


YACK_UTEST(proto_diff1d)
{
    randomized::shared_bits      sran = new randomized::mt19937();
    randomized::gaussian<double> gran( sran );

    size_t         N = 10000;
    vector<double> x(N);
    ios::ocstream  fp("diff1d.dat");
    x[1] = 0;
    fp("1 %g\n",x[1]);
    for(size_t i=2;i<=N;++i)
    {
        x[i] = x[i-1] + gran();
        fp("%lu %.15g\n",static_cast<unsigned long>(i), x[i]);
    }

    

}
YACK_UDONE()

