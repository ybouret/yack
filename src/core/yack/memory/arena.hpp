
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        class chunk;
        class allocator;

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
           
            arena(const size_t block_size, allocator &); //!< setup with a first chunk
            ~arena() throw();               //!< cleanup
            
            void *acquire();                   //!< acquire one block, zeroed
            void  release(void *addr) throw(); //!< release one block
            void  expunge(void *addr) throw(); //!< zero and release block
         
            template <typename T> inline
            T *zombie()
            {
                assert(sizeof(T)==chunk_block_size);
                return static_cast<T*>( acquire() );
            }
            
            //! acquire and default construct
            template <typename T> inline
            T *invoke() {
                assert(sizeof(T)==chunk_block_size);
                void *block = acquire();
                try        { return new(block) T(); }
                catch(...) { expunge(block); throw; }
            }

            //! destruct and expunge
            template <typename T> inline
            void revoke(T *args) throw()
            {
                assert(sizeof(T)==chunk_block_size);
                assert(NULL!=args);
                args->~T();
                expunge(args);
            }
            
        private:
            size_t       available_blocks;
            chunk       *acquiring;
            chunk       *releasing;
            void        *impl[list_words];
            allocator   &memory_io;
            
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
            void  find(void *)  throw(); //!< find releasing
        };
        
    }
    
}


#endif
