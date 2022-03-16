#include "yack/concurrent/queue/agent.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

YACK_UTEST(sync_queue)
{
    YACK_SIZEOF(concurrent::agent);

    concurrent::agency Q(4);

}
YACK_UDONE()
