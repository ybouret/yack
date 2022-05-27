#include "yack/concurrent/queue/agent.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

YACK_UTEST(sync_queue)
{
    YACK_SIZEOF(concurrent::agent);
    const concurrent::topology topo(1);

    std::cerr << "topo=" << topo << std::endl;
    
}
YACK_UDONE()
