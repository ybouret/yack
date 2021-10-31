#include "yack/concurrent/split1d.hpp"
#include "yack/concurrent/tess2d.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/heap.hpp"

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
        const int h = twice(y)+1;
        for(int x=0;x<=10;++x)
        {
            const int w = twice(x)+1;
            vector< v2d<int> > vertices(w*h,as_capacity);
            const v2d<int> lower(-x,-y);
            const v2d<int> upper(x,y);
            for(int j=lower.y;j<=upper.y;++j)
            {
                for(int i=lower.x;i<=upper.x;++i)
                {
                    const v2d<int> v(i,j);
                    YACK_ASSERT(vertices.available());
                    vertices.push_back(v);
                }
            }
            
            hsort(vertices,v2d<int>::compare);
            for(size_t i=1;i<vertices.size();++i)
            {
                for(size_t j=i+1;j<=vertices.size();++j)
                {
                    YACK_ASSERT(vertices[i]!=vertices[j]);
                }
            }
            vector< v2d<int> > generated(w*h,as_capacity);

            for(size_t size=1;size<=9;++size)
            {
                concurrent::tess2D<int> tess(lower,upper,size);
                generated.free();
                for(size_t rank=1;rank<=size;++rank)
                {
                    const concurrent::tiles2D<int> &tiles = tess[rank];
                    for(const concurrent::tile2D<int> *t=tiles.head();t;t=t->next)
                    {
                        int      length = t->width;
                        v2d<int> curr   = t->start;
                        while(length-- > 0)
                        {
                            generated.push_back(curr);
                            curr.x++;
                        }
                    }
                }
                YACK_ASSERT(generated.size() == vertices.size() );
                hsort(generated,v2d<int>::compare);
                for(size_t i=generated.size();i>0;--i)
                {
                    YACK_ASSERT( (generated[i]-vertices[i]).norm2() <= 0);
                }
            }
            
        }
    }
    
}
YACK_UDONE()


