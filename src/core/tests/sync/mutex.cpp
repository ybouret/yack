


#include "yack/synchronic/mutex.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(sync_mutex)
{
    
    synchronic::primitive::display();
    
    std::cerr << "    ==== enter " << program << std::endl;
    {
        synchronic::mutex m1;
    }
    {
        YACK_GIANT_LOCK();
    }
    std::cerr << "    ==== leave " << program << std::endl;
    
}
YACK_UDONE()

