
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
}

YACK_UTEST(counting_part)
{
    size_t       n     = 4;
    
    if(argc>1)
    {
        n = atol(argv[1]);
    }
    
    {
        yack_part        param;
        vector<size_t > ints(n,0);
        yack_part_init(&param,n);
        size_t count = 0;
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
    //apn Bell = 0;
    do
    {
        std::cerr << part << std::endl;
        //apn tmp =apn::comb(n,part.size());
        //std::cerr << "+" << tmp << std::endl;
       // Bell += tmp;
        
    } while( part.next() );
    //std::cerr << "Bell=" << Bell << std::endl;
    
}
YACK_UDONE()

