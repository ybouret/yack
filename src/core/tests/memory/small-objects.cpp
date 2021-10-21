
#include "yack/memory/small/object-allocator.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/cxx-list.hpp"
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
        size_t size;
        
        inline  block( ) throw() : next(0), prev(0), addr(0), size(0)
        {
        }
        
        inline ~block() throw()
        {
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
    
    
}

YACK_UTEST(memory_objects)
{
    concurrent::single::verbose = true;
    //concurrent::mutex::verbose  = true;

    randomized::rand_  ran;
    {
        memory::small_objects objs(32);
        cxx_list_of<block>    blocks;

        for(size_t i=1000+ran.leq(1000);i>0;--i)
        {
            block *b = blocks.push_back( new block() );
            b->addr = objs.acquire_unlocked( b->size = 1 + ran.leq(63) );
        }
        std::cerr << "objs.put_int_ram=" << objs.put_in_ram << std::endl;
        randomized::shuffle::list(blocks,ran);
        while(blocks.size)
        {
            block *b = blocks.pop_back();
            objs.release_unlocked(b->addr,b->size);
        }
        YACK_CHECK(objs.put_in_ram==0);

        YACK_SIZEOF(memory::small_objects);
    }

    {
        memory::small_object_allocator<32> &mgr =  memory::small_object_allocator<32>::instance();
        std::cerr << "call_sign=" << mgr.call_sign << std::endl;
    }


}
YACK_UDONE()
