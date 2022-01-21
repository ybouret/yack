
#include "yack/utest/run.hpp"
#include "yack/counting/c/comb.h"
#include "yack/counting/comb.cpp"
#include "yack/sequence/vector.hpp"
#include "yack/apex/natural.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;


YACK_UTEST(counting_comb)
{
    
    
    size_t n=8;
    size_t k=5;
    
    if(argc>2)
    {
        n = atol(argv[1]);
        k = atol(argv[2]);
    }
    
    {
        yack_comb       param;
        yack_comb_init(&param,n,k);
        const apn nc = apn::comb(n,k);
        std::cerr << "comb(" << n << "," << k << ")=" << nc << std::endl;
        
        if(nc>0)
        {
            vector<size_t> comb(k,0);
            yack_comb_boot(&param,*comb);
            size_t count = 0;
            do
            {
                ++count;
                std::cerr << comb  << std::endl;
            } while( yack_comb_next(&param,*comb) );
            YACK_CHECK(count==nc);
        }
    }
    
    combination comb(n,k);
    std::cerr << "comb.size()    = " << comb.size() << std::endl;
    std::cerr << "comb.granted() = " << comb.granted() << std::endl;
    comb.boot();
    do
    {
        std::cerr << comb << std::endl;
    } while(comb.next());
    
    

    
    
}
YACK_UDONE()

