#include "yack/sequence/stats.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_stats)
{
    {
        const int arr[4] = { 1, 1, 1, 1 };
        YACK_CHECK(1==statistical::average<int>::of_(arr,4));
    }

    vector<double> temp;
    vector<double> x;
    x.push_back(2);
    x.push_back(4);
    const double mu   = statistical::average<double>::of(x);
    const double sig2 = statistical::variance<double>::of(x,mu);
    std::cerr <<  "mu   = "  << mu  << std::endl;
    std::cerr <<  "sig2  = " << sig2 << std::endl;

    const double mu_prime   = statistical::average<double>::of(x,temp);
    const double sig2_prime = statistical::variance<double>::of(x,mu_prime,temp);
    std::cerr <<  "mu    = "  << mu_prime   << std::endl;
    std::cerr <<  "sig2  = "  << sig2_prime  << std::endl;



}
YACK_UDONE()

