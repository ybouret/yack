
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        class chunk;
        
        //______________________________________________________________________
        //
        //
        //! arena of chunks with same block size
        //
        //______________________________________________________________________
        class arena
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const size_t list_words = 5; //!< for internal memory
           
            arena(const size_t block_size); //!< setup with a first chunk
            ~arena() throw();               //!< cleanup
            
            void *acquire();                   //!< acquire one block
            void  release(void *addr) throw(); //!< release one block
            
        private:
            size_t       available_blocks;
            chunk       *acquiring;
            chunk       *releasing;
            void        *impl[list_words];
            
        public:
            const size_t chunk_block_size; //!< the same block size
            const size_t blocks_per_chunk; //!< for each chunk
            const size_t memory_per_chunk; //!< a power of two
            const size_t memory_signature; //!< integer log 2
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
            void  grow();                //!< with a new chunk
            void  kill(chunk *) throw(); //!< return memory
            void *give()        throw(); //!< by acquiring
            void  take(void *)  throw(); //!< by releasing
            
        };
        
    }
    
}


#endif
