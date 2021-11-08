
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/joint/parcel.hpp"
#include "yack/memory/allocator/legacy.hpp"

namespace yack
{
    namespace memory
    {

        void * legacy:: acquire(const size_t n)
        {
            if(n>0)
            {
                static allocator &mgr = pooled::instance();
                size_t count = 1;
                return mgr.acquire(count,n);
            }
            else
            {
                return NULL;
            }
        }

        void *legacy:: c_alloc(const size_t n) throw()
        {
            try
            {
                return acquire(n);
            }
            catch(...)
            {
                return NULL;
            }
            
        }

        void legacy:: release(void *addr) throw()
        {
            if(addr)
            {
                static allocator &mgr = pooled::location();
                size_t size = parcel::size_of(addr);
                mgr.release(addr,size);
            }

        }
    }

}

