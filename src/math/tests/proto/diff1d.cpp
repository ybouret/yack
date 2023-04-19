#include "yack/utest/run.hpp"
#include "yack/randomized/mt19937.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/randomized/gaussian.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/cameo/add.hpp"

using namespace yack;
using namespace math;

double X2(const readable<double> &x, const double mu, cameo::add<double> &xadd )
{
    const size_t n = x.size(); YACK_ASSERT(n>0);
    xadd.resume(n);
    for(size_t i=n;i>0;--i) {
        xadd.grow( squared(x[i]-mu) );
    }
    return xadd.sum()/n;
}

YACK_UTEST(proto_diff1d)
{
    size_t         N     = 50000;
    size_t         every = 500;

    randomized::shared_bits      sran = new randomized::mt19937();
    //randomized::shared_bits      sran = new randomized::rand_();
    //randomized::shared_bits      sran = new randomized::ParkMiller();
    randomized::gaussian<double> gran( sran );
    cameo::add<double>           xadd(N);

    vector<double> x(N,as_capacity);
    ios::ocstream  fp("diff1d.dat");
    ios::ocstream  fd("diff1d-stat.dat");
    x << 0;
    fp("1 %g\n",x[1]);
    size_t count = 1;
    for(size_t i=2;i<=N;++i)
    {
        const double xold = x.back();
        x <<  xold + (sran->choice() ? 1.0 : -1.0);
        //x << xold + gran();
        YACK_ASSERT(i==x.size());
        fp("%lu %.15g\n",static_cast<unsigned long>(i), x.back());
        fp.flush();
        
        if(++count>=every)
        {
            const double mu = xadd.tableau(x)/x.size();
            const double x2 = X2(x,mu,xadd);
            fd("%lu %.15g %.15g\n",static_cast<unsigned long>(i), mu, x2);
            fd.flush();
            count=0;
            std::cerr << "#" << x.size() << std::endl;
        }
    }




}
YACK_UDONE()

