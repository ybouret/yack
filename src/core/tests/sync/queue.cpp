#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/env.hpp"

#include <cmath>

using namespace yack;

namespace
{
    struct dummy
    {
        static const size_t RUNMIN = 1000000;
        randomized::bits &ran;
        wtime            &tmx;

        void something(lockable &sync)
        {

            size_t i=0;
            { YACK_LOCK(sync); i = RUNMIN + ran.leq(10*RUNMIN); }
            double sum = 0;
            while(i-- > 0)
            {
                sum += cos( squared(double(i)) );
            }
            {
                YACK_LOCK(sync);
                (std::cerr << "\t\tsum=" << sum << std::endl).flush();
            }
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

    YACK_SIZEOF(concurrent::drone);
    YACK_SIZEOF(concurrent::pipeline);



    size_t n=10;
    if(argc>1)
    {
        n = ios::ascii::convert::to<size_t>(argv[1],"n");
    }

    const concurrent::topology topo;
    concurrent::thread::verbose = environment::flag("VERBOSE");

    std::cerr << "topo=" << topo << std::endl;
    std::cerr << "-------- Empty Q --------" << std::endl;
    {
        concurrent::pipeline empty(topo);
    }


    concurrent::pipeline Q(topo);

    std::cerr << std::endl;
    std::cerr << "-------- using Q v1 --------" << std::endl;

    dummy D = { ran, tmx };
    Q(&D, & dummy::something );
    Q(D);
    Q(&D, & dummy::something );
    Q(D);
    Q.flush();
    

    std::cerr << "-------- using Q v2 --------" << std::endl;
    for(size_t i=1;i<=n;++i)
    {
        Q(&D, &dummy::something);
    }
    Q.flush();

    std::cerr << "-------- final Q    --------" << std::endl;


}
YACK_UDONE()
