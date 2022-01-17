
#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"
#include "yack/counting/c/part.h"
#include "yack/sequence/vector.hpp"
#include "yack/apex/natural.hpp"

using namespace yack;


namespace
{
    static inline void show(const readable<size_t> &ints, const size_t k)
    {
        std::cerr << '[';
        std::cerr << ints[1];
        for(size_t i=2;i<=k;++i)
        {
            std::cerr << ';' << ints[i];
        }
        std::cerr << ']' << std::endl;
    }

    size_t call_p(size_t n, size_t k)
    {
        //std::cerr << "call_p(" << n << "," << k << ")" << std::endl;
        if(1==k)
        {
            return 1;
        }
        else
        {

            switch( __sign::of(k,n) )
            {
                case negative: assert(k<n);  return call_p(n-1,k-1)+call_p(n-k,k);
                case __zero__: assert(k==n); return 1;
                case positive: assert(k>n);  return 0;
            }
            return 0;
        }
    }

    size_t give_p(size_t n)
    {
        assert(n>0);
        size_t sum = 0;
        for(size_t k=1;k<=n;++k)
        {
            sum += call_p(n,k);
        }
        return sum;
    }


}

YACK_UTEST(counting_part)
{
    size_t       n     = 4;
    
    if(argc>1)
    {
        n = atol(argv[1]);
    }

    const size_t np = give_p(n);
    std::cerr << "p(" << n << ")=" << np << std::endl;

    size_t count = 0;
    {
        yack_part        param;
        vector<size_t > ints(n,0);
        yack_part_init(&param,n);
        yack_part_boot(&param,*ints);
        do
        {
            ++count;
            show(ints,param.k);
        } while( yack_part_next(&param,*ints) );
        std::cerr << "#count=" << count << std::endl;
    }
    
    partition part(n);
    std::cerr << "part.size()    = " << part.size() << std::endl;
    std::cerr << "part.granted() = " << part.granted() << std::endl;
    
    part.boot();
    do
    {
        std::cerr << part << std::endl;
    } while( part.next() );
    std::cerr << count << "/" << np << std::endl;
}
YACK_UDONE()

