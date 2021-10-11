
#include "yack/memory/parcels.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"

using namespace yack;

namespace
{
    class block
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        size_t size;
        inline  block(void*p, size_t n) throw() : next(0), prev(0), addr(p), size(n) {}
        inline ~block() throw() {}
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
}

YACK_UTEST(memory_parcels)
{
    uprng              ran;
    {
        memory::parcels    mgr;
        cxx_list_of<block> blocks;
        
        std::cerr << "creating parcels..." << std::endl;
        for(size_t i=0;i<100;++i)
        {
            size_t n = ran.leq(20000);
            void  *p = mgr.acquire_unlocked(n);
            blocks.push_back( new block(p,n) );
        }
        
        mgr.display();
        
        std::cerr << "deleting parcels..." << std::endl;
        ran.shuffle_list(blocks);
        while(blocks.size)
        {
            block *blk = blocks.pop_back();
            mgr.release_unlocked(blk->addr,blk->size);
            delete blk;
        }
        mgr.display();
        
    }
    std::cerr << "status" << std::endl;
    memory::dyadic::instance().display();
    
    
    
}
YACK_UDONE()


