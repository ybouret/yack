
//! \file

#ifndef YACK_MEMORY_SCARCE_INCLUDED
#define YACK_MEMORY_SCARCE_INCLUDED 1

#include "yack/memory/allocator.hpp"

namespace yack
{
    
    namespace memory
    {
        class scarce : public allocator
        {
        public:
            static const size_t max_block_exp2 = (sizeof(size_t) << 3)  - 1;
            static const size_t max_block_size = size_t(1) <<max_block_exp2;
            static const size_t arenas         = max_block_exp2+1;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scarce);
            explicit scarce() throw();
            virtual ~scarce() throw();
        };
    
    }
    
}

#endif

