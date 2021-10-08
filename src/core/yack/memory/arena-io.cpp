
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/memory/allocator/pages.hpp"

namespace yack
{
    namespace memory
    {
        
        void arena:: gc(pages &target) throw()
        {
            YACK_LOCK(target.access);
            while(reservoir.size)
            {
                target.store_large_unlocked(reservoir.pop(),frame_exp2);
            }
        }
    
    }
    
}

