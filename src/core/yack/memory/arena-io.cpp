
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace memory
    {
        
        void arena:: gc(dyadic &target) throw()
        {
            target.book_store_pool(reservoir,frame_exp2);
        }
    
    }
    
}

