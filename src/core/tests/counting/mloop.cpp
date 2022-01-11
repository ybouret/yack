#include "yack/counting/mloop.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(counting_loop)
{

    const int ini[] = { 1, 3, 4};
    const int end[] = { 2, 3, 0};

    mloop<int> loop(ini,end,3);
    loop.display();

    std::cerr << "loop=" << loop << std::endl;
    loop.boot();
    std::cerr << "loop=" << loop << std::endl;
    while( loop.next() )
    {
        std::cerr << "loop=" << loop << std::endl;
    }


}
YACK_UDONE()

