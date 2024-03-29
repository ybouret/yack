#include "yack/ptr/contractor.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace  yack;

YACK_UTEST(memory_contractor)
{
    uint64_t toto[128];
    {
        contractor<int> ic(toto,4);
        for(size_t i=1;i<=ic.amount;++i)
        {
            ic[i] = -1;
        }
    }
    
    YACK_SIZEOF(string);

    {
        contractor<string> sc(toto,2);
    }

    return 0;
    
    {
        contractor<string> sc(toto,2);
    }


}
YACK_UDONE()

