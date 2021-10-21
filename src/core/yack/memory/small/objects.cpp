#include "yack/memory/small/objects.hpp"
#include "yack/memory/small/blocks.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/out-of-reach.hpp"
#include <new>
#include <iostream>

namespace yack
{
    namespace memory
    {

        const char small_objects::designation[] = "memory::small_objects";
        
        small_objects :: ~small_objects() throw()
        {
            if(0!=put_in_ram)
            {
                std::cerr << "*** [memory::objects::put_int_ram=" << put_in_ram << "]" << std::endl;
            }
            out_of_reach::zset(destructed(blk),sizeof(impl));
            blk = 0;
        }
        
        small_objects:: small_objects(const size_t the_limit_size) :
        limit_size(the_limit_size),
        put_in_ram(0),
        blk(NULL),
        impl()
        {
            blk = new ( out_of_reach::zset(impl,sizeof(impl)) ) blocks();
        }
        
        void * small_objects:: acquire_unlocked(size_t block_size)
        {
            assert(block_size>0);
            if(block_size<=limit_size)
            {
                return blk->acquire(block_size);
            }
            else
            {
                static allocator &mgr = global::instance();
                size_t            num = 1;
                void             *ptr = mgr.acquire(num,block_size);
                assert(block_size==num);
                coerce(put_in_ram) += block_size;
                return ptr;
            }
        }
        
        void  small_objects:: release_unlocked(void *block_addr, size_t block_size) throw()
        {
            assert(block_addr);
            assert(block_size>0);
            if(block_size<=limit_size)
            {
                  blk->release(block_addr,block_size);
            }
            else
            {
                static allocator &mgr = global::location();
                coerce(put_in_ram) -= block_size;
                mgr.release(block_addr,block_size);
            }
        }

        void  small_objects:: gc_unlocked(dyadic &target) throw()
        {
            blk->gc(target);
        }

        
    }
    
}
