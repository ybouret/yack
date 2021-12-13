#include "yack/sequence/stats.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_stats)
{
    const int arr[4] = { 1, 1, 1, 1 };
    YACK_CHECK(1==statistical::average<int>::of_(arr,4));
    
}
YACK_UDONE()

