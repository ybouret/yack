
#include "yack/memory/small/arena.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace memory
    {
        
        void arena:: gc(dyadic &target) noexcept
        {
            target.retrieve(reservoir,frame_exp2);
        }
    
    }
    
}

