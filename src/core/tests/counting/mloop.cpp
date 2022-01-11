#include "yack/counting/mloop.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(counting_loop)
{

    const int ini[] = { 1, 3, 4};
    const int end[] = { 2, 3, 0};

    mloop<int> loop(ini,end,3);
    std::cerr << "granted=" << loop.granted() << std::endl;
    std::cerr << "loop=" << loop << std::endl;

    loop.display();

}
YACK_UDONE()

