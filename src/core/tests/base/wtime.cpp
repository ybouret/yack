
#include "yack/system/wtime.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(wtime)
{

    wtime rtc;
    std::cerr << "rtc.freq=" << rtc.freq << std::endl;

    for(size_t i=0;i<10;++i)
    {
        std::cerr << wtime::ticks() << std::endl;
    }

    std::cerr << "wait(0.5)..." << std::endl;
    rtc.wait(0.5);
    std::cerr << "...done" << std::endl;

}
YACK_UDONE()
