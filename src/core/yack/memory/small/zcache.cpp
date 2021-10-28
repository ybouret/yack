#include "yack/memory/small/zcache.hpp"
#include "yack/memory/small/arena.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/out-of-reach.hpp"

#include <cstring>
#include <new>

namespace yack
{
    namespace memory
    {
        zcache:: ~zcache() throw()
        {
            free();
            well->display_arena();
            out_of_reach::zset(destructed(well),sizeof(impl));
        }
        
        
        size_t zcache:: block_size_for(const size_t bs) throw()
        {
            const size_t mbs = max_of( sizeof(stub), bs );
            return YACK_ALIGN_TO(void *,mbs);
        }
        
        zcache:: zcache(const size_t bs) :
        block_size( block_size_for(bs) ),
        repo(),
        well(0),
        impl()
        {
            static memory::allocator &mem     = memory::global::instance();
            static const bool         compact = true;
            
            memset(&repo,0,sizeof(repo));
            well = new ( memset(impl,0,sizeof(impl)) ) arena(block_size,mem,compact);
            
        }
        
        void zcache:: load(size_t n)
        {
            while(n-- > 0 )
                repo.push( static_cast<stub*>(well->acquire()) );
        }
        
        void zcache:: free() throw()
        {
            while( repo.size )
                well->release( repo.pop() );
        }
        
        void * zcache:: acquire_unlocked()
        {
            if(repo.size)
            {
                return memset(repo.pop(),0,block_size);
            }
            else
            {
                return well->acquire();
            }
        }
        
        void  zcache:: release_unlocked(void *addr) throw()
        {
            assert(addr);
            repo.push( static_cast<stub *>(memset(addr,0,sizeof(stub))));
        }
        
    }
}
