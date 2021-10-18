

#include "yack/type/temporary.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(type_temp)
{

    int a = 5;
    std::cerr << "a=" << a << std::endl;
    {
        const temporary<int> guard(a,12);
        std::cerr << "a=" << a << std::endl;
    }
    std::cerr << "a=" << a << std::endl;

    int *p = NULL;
    {
        const temporary<int*> guard(p,&a);
        YACK_ASSERT(p==&a);
    }
    YACK_ASSERT(!p);


}
YACK_UDONE()
