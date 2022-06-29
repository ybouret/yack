#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(sync_queue)
{
    const concurrent::topology topo;
    concurrent::thread::verbose = true;

    std::cerr << "topo=" << topo << std::endl;
    concurrent::pipeline Q(topo);

    
}
YACK_UDONE()
