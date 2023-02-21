#include "yack/memory/small/zcache.hpp"
#include "yack/memory/small/arena.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/system/error.hpp"
#include "yack/type/cstring.h"

#include <cstring>
#include <new>
#include <cerrno>

namespace yack
{
    namespace memory
    {
        
        

        void zcache:: xdisabled(const char *clid)
        {
            char msg[256];
            assert(clid);
            yack_cstring_msgcpy(msg,sizeof(msg),"disabled for ");
            yack_cstring_msgcat(msg,sizeof(msg),clid);
            system_error::critical_bsd(EINVAL,msg);
        }

        
        zcache:: ~zcache() noexcept
        {
            free();
            //well->display_arena();
            out_of_reach::zset(destructed(well),sizeof(impl));
        }
        
        
        size_t zcache:: block_size_for(const size_t bs) noexcept
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

        size_t  zcache:: here() const noexcept
        {
            return repo.size;
        }


        void zcache:: load(size_t n)
        {
            while(n-- > 0 )
                repo.push( static_cast<stub*>(well->acquire()) );
        }
        
        void zcache:: free() noexcept
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
        
        void  zcache:: release_unlocked(void *addr) noexcept
        {
            assert(addr);
            repo.push( static_cast<stub *>(memset(addr,0,sizeof(stub))));
        }
        
    }
}
