
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        
        class arena
        {
        public:
            explicit arena(const size_t block_size) throw();
            virtual ~arena() throw();
            
            size_t       available_chunks;
            const size_t chunk_block_size;
            const size_t blocks_per_chunk;
            const size_t memory_per_chunk;
            const size_t memory_signature;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
        };
        
    }
    
}


#endif
