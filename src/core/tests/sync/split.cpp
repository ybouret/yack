#include "yack/concurrent/split1d.hpp"
#include "yack/concurrent/split2d.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(sync_split)
{
    v2d<int> lower(-5,0);
    v2d<int> upper(10,0);
    
    for(size_t size=1;size<=6;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            concurrent::tiling<int> tiling;
            concurrent::split2D::build(tiling,size,rank,lower,upper);
            std::cerr << "\t." << rank << ":";
            std::cerr << " #=" << tiling.size;
            for(const concurrent::tile<int> *t=tiling.head;t;t=t->next)
            {
                std::cerr << " " << t->start << "+" << t->width;
            }
            std::cerr << std::endl;
        }
    }
}
YACK_UDONE()


