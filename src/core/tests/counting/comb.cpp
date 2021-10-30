
#include "yack/utest/run.hpp"
#include "yack/counting/c/comb.h"
#include "yack/counting/comb.cpp"
#include "yack/sequence/vector.hpp"
#include "yack/apex/natural.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;


namespace
{
    static inline
    void compute_comb_limits()
    {
        ios::ocstream::overwrite("comb32.dat");
        ios::ocstream::overwrite("comb64.dat");
        for(size_t n=1;;++n)
        {
            list<size_t> gt32;
            list<size_t> gt64;
            for(size_t k=1;k<=n;++k)
            {
                const apn    nc = apn::comb(n,k);
                const size_t nb = nc.bits();
                if(nb>32)
                {
                    gt32.push_back(k);
                }
                if(nb>64)
                {
                    gt64.push_back(k);
                }
            }
            if(gt32.size())
            {
                std::cerr << "( * ) n=" << n << ": " << gt32.front() << " -> " << gt32.back() << std::endl;
                ios::ocstream::echo("comb32.dat","%u %u %u\n",unsigned(n),unsigned(gt32.front()),unsigned(gt32.back()));
            }
            if(gt64.size())
            {
                std::cerr << "(***) n=" << n << ": " << gt64.front() << " -> " << gt64.back() << std::endl;
                ios::ocstream::echo("comb64.dat","%u %u %u\n",unsigned(n),unsigned(gt64.front()),unsigned(gt64.back()));
            }
            
            if(n>=80)
                break;
        }
    }
}

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
    
    
    
    
    if(false)
        compute_comb_limits();
    
    
}
YACK_UDONE()

