#include "yack/sequence/stats.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/shuffle.hpp"

using namespace yack;

YACK_UTEST(data_stats)
{
    randomized::rand_ ran;

    {
        math::adder<int> iadd;
        const int arr[4] = { 1, 1, 1, 1 };
        YACK_CHECK(1==statistical::average<int>::of(arr,4,iadd));
    }

    math::adder<double> xadd;
    {

        double arr[8] = { 1, 2, 3, 4, 5, 6, 7,  8 };

        for(size_t n=0;n<=8;++n)
        {
            for(size_t i=1;i<=n;++i)
            {
                arr[i-1] = i;
            }
            randomized::shuffle::data(arr,n, ran);
            std::cerr << "arr=";
            for(size_t i=0;i<n;++i) std::cerr << ' ' << arr[i];
            std::cerr << std::endl;
            
            const double m = xadd.median(arr,n);;
            std::cerr << "m" << n << " = " << m << std::endl;
        }
        std::cerr << std::endl;
    }

    vector<double> x;
    x.push_back(2);
    x.push_back(4);

    const double mu   = statistical::average<double>::of(x,xadd);
    const double sig2 = statistical::variance<double>::of(x,mu,xadd);
    const double med  = xadd.median(x);
    std::cerr <<  "mu      = "  << mu   << std::endl;
    std::cerr <<  "sig2    = " << sig2 << std::endl;
    std::cerr <<  "median  = " << med << std::endl;


}
YACK_UDONE()

