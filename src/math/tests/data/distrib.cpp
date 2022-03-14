
#include "yack/sequence/vector.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/randomized/gaussian.hpp"

#include "yack/sort/heap.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;
//using namespace math;

namespace
{
    static inline void build_dist(writable<double> &x, const char *filename)
    {
        const size_t n = x.size(); assert(n>0);
        const size_t h = n>>1;
        hsort(x,comparison::increasing<double>);
        const double med = (n&1) ? x[h+1] : 0.5*(x[h]+x[h+1]);
        std::cerr << "median=" << med << std::endl;
        vector<double> d(n,as_capacity);
        for(size_t i=n;i>0;--i)
        {
            d << fabs(x[i]-med);
        }
        hsort(d,comparison::increasing<double>);

        {
            ios::ocstream fp(filename);
            for(size_t i=1;i<=n;++i)
            {
                fp("%.15g %.15g\n", d[i], double(i)/(n+1));
            }
        }
        std::cerr << "D=" << sorted::sum(d,sorted::by_value)/n << std::endl;
    }



}

YACK_UTEST(distrib)
{
    randomized::shared_bits      ran = new randomized::rand_();
    randomized::gaussian<double> gran( ran );

    vector<double> x;
    vector<double> temp;

    const size_t nn = 10000;

    x.free();
    for(size_t i=nn;i>0;--i)
    {
        x.push_back( gran() );
    }
    assert(nn==x.size());
    build_dist(x,"gaussian.dat");

    


    x.free();
    for(size_t i=nn;i>0;--i)
    {
        x.push_back( (*ran)() );
    }
    assert(nn==x.size());
    build_dist(x,"uniform.dat");


}
YACK_UDONE()
