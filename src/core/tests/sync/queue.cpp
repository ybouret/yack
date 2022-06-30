#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

namespace
{
    struct dummy
    {
        randomized::bits &ran;
        wtime            &tmx;

        void something(lockable &sync)
        {
            YACK_LOCK(sync);

        }

        void operator()(lockable &sync)
        {
            YACK_LOCK(sync);
        }
    };
}

YACK_UTEST(sync_queue)
{
    randomized::rand_ ran;
    wtime             tmx;

    YACK_SIZEOF(concurrent::pipeline::drone);
    YACK_SIZEOF(concurrent::pipeline);

    size_t n=10;
    if(argc>1)
    {
        n = ios::ascii::convert::to<size_t>(argv[1],"n");
    }

    const concurrent::topology topo;
    concurrent::thread::verbose = true;

    std::cerr << "topo=" << topo << std::endl;
    concurrent::pipeline Q(topo);

    std::cerr << std::endl;
    std::cerr << "-------- using Q v1 --------" << std::endl;

    dummy D = { ran, tmx };
    Q(&D, & dummy::something );
    Q(&D, & dummy::something );
    Q(D);
    Q.flush();
    

    std::cerr << "-------- using Q v2 --------" << std::endl;
    for(size_t i=1;i<=n;++i)
    {

    }

    std::cerr << "-------- final Q    --------" << std::endl;


}
YACK_UDONE()
