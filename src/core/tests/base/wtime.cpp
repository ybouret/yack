
#include "yack/system/wtime.hpp"
#include "yack/system/pid.hpp"
#include "yack/system/hw.hpp"
#include "yack/system/seed.hpp"
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

    std::cerr << "in pid: " << process_id::get()  << std::endl;
    std::cerr << "#cpu  : " << hardware::nprocs() << std::endl;

    std::cerr << "Seeds..." << std::endl;
    std::cerr << std::hex;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << system_seed::get64() << std::endl;
    }
    std::cerr << std::dec;
    std::cerr << system_seed::get<char>() << std::endl;
    std::cerr << system_seed::get<short>() << std::endl;
    std::cerr << system_seed::get<int>() << std::endl;
    std::cerr << system_seed::get<long>() << std::endl;
    std::cerr << system_seed::get<long long>() << std::endl;


}
YACK_UDONE()
