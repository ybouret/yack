
#include "yack/utest/run.hpp"
#include "yack/counting/perm.h"
#include "yack/counting/perm.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/apex/natural.hpp"

using namespace yack;



YACK_UTEST(counting_perm)
{
    size_t       n     = 7;
    
    if(argc>1)
    {
        n = atol(argv[1]);
    }
    
    {
        const apn    nf = apn::factorial(n);
        const size_t np = nf.cast_to<size_t>("perm.count");
        
        std::cerr << "#perm(" << n << ")=" << np << std::endl;
        
        vector<size_t> perm(n,0);
        yack_perm      param;
        yack_perm_init(&param,n);
        yack_perm_boot(&param,*perm);
        size_t count=0;
        do {
            ++count;
            std::cerr << perm << std::endl;
        } while(yack_perm_next(&param,*perm));
        std::cerr << "#perm=" << count << std::endl;
        YACK_CHECK(np==count);
    }
    
    permutation perm(n);
    std::cerr << "perm.size()    = " << perm.size() << std::endl;
    std::cerr << "perm.granted() = " << perm.granted() << std::endl;
    
    perm.boot();
    do
    {
        std::cerr << perm << std::endl;
    } while( perm.next() );
    
    
}
YACK_UDONE()


