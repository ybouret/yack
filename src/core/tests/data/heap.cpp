#include "yack/ordered/heap.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;

    heap<double,3> hd3;
    std::cerr << hd3.granted() << " / " << hd3.wksp_size << std::endl;
}
YACK_UDONE()
