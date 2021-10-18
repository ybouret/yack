#include "yack/concurrent/loop/mono.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/concurrent/thread.hpp"

#include <cstdlib>

using namespace yack;

namespace
{

    static inline
    void compute(const concurrent::context &here,
                 void                      *args,
                 lockable                  &sync)
    {
        {
            YACK_LOCK(sync);
            std::cerr << "  (*) Computing with " << here << std::endl;
        }

    }


}

YACK_UTEST(sync_loop)
{
    uprng ran;

    concurrent::thread::verbose = true;
    int        threads = 1;
    if(argc>1) threads = atoi(argv[1]);

    if(false)
    {
        int load = 6;
        int init = 1;

        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "size=" << size << std::endl;
            int total = 0;
            for(size_t rank=0;rank<size;++rank)
            {
                int todo = load;
                int from = init;
                concurrent::split::with(size,rank,todo,from);
                std::cerr << "\t" << size << "." << rank << " @" << from << "+" << todo << std::endl;
                total += todo;
            }
            YACK_ASSERT(total==load);
        }
    }

    


    concurrent::mono seq;
    concurrent::simd par(threads);

    seq(compute,NULL);

    for(size_t i=0;i<16;++i)
    {
        par(compute,NULL);
    }


}
YACK_UDONE()



