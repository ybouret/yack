#include "yack/data/small/repo.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace
{
    
}

YACK_UTEST(data_small_repo)
{
    small_repo<int> icache;
    icache.push_back(5);
    icache.push_front(7);
    icache.push_back(13);
    std::cerr << *icache << std::endl;
    icache->reverse();
    std::cerr << *icache << std::endl;
    
    icache.free();

}
YACK_UDONE()


