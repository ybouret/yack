#include "yack/ptr/auto.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"

using namespace yack;

YACK_UTEST(memory_ptr)
{
    std::cerr << ptr_::nil << std::endl;
    
    {
        auto_ptr<apq> nil;
        std::cerr << nil << std::endl;
    }
    
    {
        auto_ptr<apq> pq = new apq(1,2);
        std::cerr << pq << std::endl;
        pq = NULL;
        std::cerr << pq << std::endl;
        pq = new apq(2,3);
        std::cerr << pq << std::endl;

    }
}
YACK_UDONE()

