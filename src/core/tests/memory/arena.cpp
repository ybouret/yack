
#include "yack/memory/ram.hpp"
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
        
        inline  block(void *p) throw() : next(0), prev(0), addr(p)
        {
        }
        
        inline ~block() throw()
        {
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
    
    static inline void check_all_different(const list_of<block> &blocks)
    {
        for(const block *a=blocks.head;a;a=a->next)
        {
            const void *lhs = a->addr;
            for(const block *b=a->next;b;b=b->next)
            {
                const void *rhs = b->addr;
                YACK_ASSERT(lhs!=rhs);
            }
        }
    }
}

YACK_UTEST(memory_arena)
{
    
    randomized::rand_  ran;
    cxx_list_of<block> blocks;
    memory::ram        mem;

    for(size_t block_size=1;block_size<=128;++block_size)
    {
        memory::arena a(block_size,mem,false);

        for(size_t i=300+ran.leq(1000);i>0;--i)
        {
            blocks.push_back( new block( a.acquire() ) );
        }
        check_all_different(blocks);
        randomized::shuffle::list(blocks,ran);
        {
            const size_t half = blocks.size/2;
            while(blocks.size>half)
            {
                block *blk = blocks.pop_back();
                a.release(blk->addr);
                delete blk;
            }
        }
        for(size_t i=300+ran.leq(1000);i>0;--i)
        {
            blocks.push_back( new block( a.acquire() ) );
        }
        check_all_different(blocks);
        randomized::shuffle::list(blocks,ran);
        while(blocks.size)
        {
            block *blk = blocks.pop_back();
            a.release(blk->addr);
            delete blk;
        }

        a.display_arena();
    }
    
    YACK_SIZEOF(memory::arena);
    std::cerr << std::endl;
    for(size_t i=0;i<=8;++i)
    {
        const size_t  block_size = 1<<i;
        memory::arena a(block_size,mem,true);
        memory::arena b(block_size,mem,false);

        a.display_arena();
        b.display_arena();
        std::cerr << std::endl;
    }
    
}
YACK_UDONE()

