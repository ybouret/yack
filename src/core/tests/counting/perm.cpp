
#include "yack/utest/run.hpp"
#include "yack/counting/perm.h"
#include "yack/sequence/vector.hpp"
#include "yack/apex/natural.hpp"

using namespace yack;


namespace
{

}

YACK_UTEST(counting_perm)
{
    size_t       n     = 4;

    if(argc>1)
    {
        n = atol(argv[1]);
    }

    const apn    nf    = apn::factorial(n);
    const size_t count = nf.cast_to<size_t>("perm.count");

    std::cerr << "#perm(" << n << ")=" << count << std::endl;

    vector<size_t> perm(n,0);
    yack_perm      param = { n };

    yack_perm_boot(&param,*perm);
    size_t index=0;
    while(true) {
        ++index;
        std::cerr << perm << std::endl;
        if(index>=count) break;
        yack_perm_next(&param,*perm);
    };



}
YACK_UDONE()


