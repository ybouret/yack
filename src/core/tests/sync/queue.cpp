#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"

using namespace yack;

namespace
{
    struct dummy
    {
        void something(lockable &sync)
        {
            YACK_LOCK(sync);

        }
    };
}

YACK_UTEST(sync_queue)
{
    YACK_SIZEOF(concurrent::pipeline::drone);
    YACK_SIZEOF(concurrent::pipeline);

    const concurrent::topology topo;
    concurrent::thread::verbose = true;

    std::cerr << "topo=" << topo << std::endl;
    concurrent::pipeline Q(topo);

    std::cerr << std::endl;
    std::cerr << "-------- using Q --------" << std::endl;

    dummy D;
    Q.enroll(&D, & dummy::something );

    //wtime chrono;
    //chrono.wait(1);

}
YACK_UDONE()
