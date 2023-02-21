
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include "yack/memory/small/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/list/cxx.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;


namespace
{
    class block
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        
        inline  block(void *p) noexcept : next(0), prev(0), addr(p)
        {
        }
        
        inline ~block() noexcept
        {
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
    
    
}

YACK_UTEST(memory_arena_io)
{
    
    randomized::rand_  ran;
    cxx_list_of<block> blocks;
    memory::allocator  &gmem = memory::global::instance();
    memory::dyadic     &pmem = memory::dyadic::instance();

    for(size_t block_size=1;block_size<=256;++block_size)
    {
        memory::arena a(block_size,gmem,false);
        for(size_t i=0;i<1000;++i)
        {
            void *p = a.acquire();
            blocks.push_back( new block(p) );
        }
        randomized::shuffle::list(blocks,ran);
        a.display_arena();
        const size_t low = blocks.size/4;
        while(blocks.size>low)
        {
            a.release(blocks.tail->addr);
            delete blocks.pop_back();
        }
        a.display_arena();
        a.gc(pmem);
        a.display_arena();
        while(blocks.size)
        {
            a.release(blocks.tail->addr);
            delete blocks.pop_back();
        }
        a.display_arena();
        a.gc(pmem);
        a.display_arena();
        pmem.display();
        std::cerr << std::endl;
    }
    
}
YACK_UDONE()

