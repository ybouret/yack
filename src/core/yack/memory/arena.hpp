
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        class chunk;
        
        class arena
        {
        public:
            static const size_t list_words = 5;
            
            arena(const size_t block_size);
            ~arena() throw();
            
            void *acquire();
            void  release(void *addr) throw();
            
        private:
            size_t       available_blocks;
            chunk       *acquiring;
            chunk       *releasing;
            void        *impl[list_words];
            
        public:
            const size_t chunk_block_size;
            const size_t blocks_per_chunk;
            const size_t memory_per_chunk;
            const size_t memory_signature;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
            void  grow();
            void  kill(chunk *) throw();
            void *give()        throw(); //!< by acquiring
            void  take(void *)  throw(); //!< by releasing
            
        };
        
    }
    
}


#endif
