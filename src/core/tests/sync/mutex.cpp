


#include "yack/synchronic/mutex.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(sync_mutex)
{
    std::cerr << "    ==== enter " << program << std::endl;
    {
        synchronic::mutex m1;
    }
    std::cerr << "    ==== leave " << program << std::endl;

}
YACK_UDONE()

