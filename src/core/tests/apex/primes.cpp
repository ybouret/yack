#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(aprimes)
{
    apex::primes & P = apex::primes::instance();
    std::cerr << P.call_sign << std::endl;

    YACK_SIZEOF(apn);
    YACK_SIZEOF(apex::prime_knot);

    std::cerr << "#primes=" << P->size << std::endl;

    {
        size_t count=0;
        for(const apex::prime_knot *node=P->head;node;node=node->next)
        {
            ++count;
            std::cerr << '#' << std::setw(5) << count << " : " <<  *node << std::endl;
        }
        std::cerr << "_I=" << P._I << std::endl;
    }


    if(argc>1)
    {
        ios::icstream fp(argv[1]);
    }

}
YACK_UDONE()

