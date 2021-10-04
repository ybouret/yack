



#include "yack/synchronic/condition.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(sync_condition)
{
    std::cerr << "    ==== enter " << program << std::endl;
    {
        synchronic::condition cond;
    }
    std::cerr << "    ==== leave " << program << std::endl;
    
}
YACK_UDONE()

