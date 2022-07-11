


#include "yack/concurrent/mutex.hpp"
#include "yack/concurrent/probe.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(sync_mutex)
{
    
    concurrent::primitive::display();
    
    std::cerr << "    ==== enter " << program << std::endl;
    {
        concurrent::mutex m1;
        YACK_LOCK_PROBE(m1,true);
    }
    {
        YACK_GIANT_LOCK();
    }
    std::cerr << "    ==== leave " << program << std::endl;
    

}
YACK_UDONE()

