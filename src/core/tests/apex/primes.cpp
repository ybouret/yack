#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(aprimes)
{
    apex::primes & P = apex::primes::instance();
    std::cerr << P.call_sign << std::endl;

    YACK_SIZEOF(apn);
    YACK_SIZEOF(apex::prime_knot);

    std::cerr << "#primes=" << P->size << std::endl;

    for(const apex::prime_knot *node=P->head;node;node=node->next)
    {
        std::cerr << *node << " -> " << (*node)._sq_ << std::endl;
    }
    std::cerr << "_I=" << P._I << std::endl;
    
    if(argc>1)
    {
        apn n = apn::parse(argv[1]);
        std::cerr << "n=" << n << std::endl;
        if( P.is(n) )
        {
            std::cerr << "prime!" << std::endl;
        }
        else
        {
            std::cerr << "not prime!" << std::endl;
        }
    }

}
YACK_UDONE()

