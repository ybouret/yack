#include "yack/container/iterator/linear.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_iter)
{
    std::cerr << iterating::direction_text(iterating::forward) << std::endl;
    std::cerr << iterating::direction_text(iterating::reverse) << std::endl;

}
YACK_UDONE()


