



#include "yack/concurrent/condition.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(sync_condition)
{
    std::cerr << "    ==== enter " << program << std::endl;
    {
        concurrent::condition cond;
    }
    std::cerr << "    ==== leave " << program << std::endl;
    
}
YACK_UDONE()

