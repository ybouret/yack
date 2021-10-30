#include "yack/concurrent/split1d.hpp"
#include "yack/concurrent/tess2d.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(sync_split)
{
    {
        v2d<int> lower(-5,-1);
        v2d<int> upper(10,1);
        
        for(size_t size=1;size<=3;++size)
        {
            std::cerr << "size=" << size << std::endl;
            for(size_t rank=0;rank<size;++rank)
            {
                concurrent::tiles2D<int> tiling;
                concurrent::split2D::build(tiling,size,rank,lower,upper);
                std::cerr << "\t." << rank << ":";
                std::cerr << " #=" << tiling.size();
                for(const concurrent::tile2D<int> *t=tiling.head();t;t=t->next)
                {
                    std::cerr << " " << t->start << "+" << t->width;
                }
                std::cerr << std::endl;
            }
        }
    }
    
    std::cerr << "large test..." << std::endl;
    for(int y=0;y<=10;++y)
    {
        for(int x=0;x<=10;++x)
        {
            const v2d<int> lower(-x,-y);
            const v2d<int> upper(x,y);
            for(size_t size=1;size<=9;++size)
            {
                concurrent::tess2D<int> tess(lower,upper,size);
                
                
            }
            
        }
    }
    
}
YACK_UDONE()


