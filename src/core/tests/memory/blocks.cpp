#include "yack/memory/blocks.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

namespace
{
    class block
    {
    public:
        inline block(void *p, size_t n) throw() : next(0), prev(0), addr(p), size(n)
        {
        }

        inline ~block() throw()
        {
        }

        block *next;
        block *prev;
        void  *addr;
        size_t size;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
}

YACK_UTEST(memory_blocks)
{
    uprng              ran;
    memory::blocks     b;
    cxx_list_of<block> l;

    for(size_t i=0;i<=10000;++i)
    {
        const size_t n = 1 + ran.leq(255);
        void        *p = b.acquire(n);
        l.push_back( new block(p,n) );
    }



}
YACK_UDONE()
