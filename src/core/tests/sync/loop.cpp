#include "yack/concurrent/loop/mono.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(sync_loop)
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


    concurrent::mono seq;
    concurrent::simd par(1);

}
YACK_UDONE()



