
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        //______________________________________________________________________
        //
        //
        // forward declarations
        //
        //______________________________________________________________________
        class chunk;
        class allocator;

        //______________________________________________________________________
        //
        //
        //! arena of chunks with same block size
        /**
         - use power of two aligned chunk frames
         - chunk can be interchanged with pages
         */
        //
        //______________________________________________________________________
        class arena
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const size_t list_words = 4; //!< for internal memory
            static const size_t pool_words = 3; //!< for internal memory

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with a first chunk
            /**
             \param block_size for all the chunks
             \param dispatcher for frames
             \param compact    for space saving if only a few items shall be in use
             */
            arena(const size_t block_size,
                  allocator   &dispatcher,
                  const bool   compact);
            ~arena() throw();               //!< cleanup

            //__________________________________________________________________
            //
            // memory methods
            //__________________________________________________________________
            void *acquire();                   //!< acquire one block, zeroed
            void  release(void *addr) throw(); //!< release one block
            void  expunge(void *addr) throw(); //!< zero and release block

            //__________________________________________________________________
            //
            // memory helpers
            //__________________________________________________________________

            //! create a zombie object
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
            size_t       available; //!< bookkeeping
            chunk       *acquiring;        //!< last acquiring
            chunk       *releasing;        //!< last releasing
            chunk       *abandoned;        //!< last empty chunk
            void        *impl[list_words]; //!< chunks list
            void        *repo[pool_words]; //!< chunks pool
            allocator   &memory_io;        //!< allocator for frames
            
        public:
            const size_t chunk_block_size; //!< the same block size
            const size_t blocks_per_chunk; //!< for each chunk
            const size_t memory_per_chunk; //!< a power of two
            const size_t memory_signature; //!< integer log 2
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
            chunk *build();               //!< a new chunk
            chunk *query();               //!< cached or new
            void   grow();                //!< with query()
            void   kill(chunk *) throw(); //!< return memory
            void  *give()        throw(); //!< by acquiring
            void   take(void *)  throw(); //!< by releasing
            void   find(void *)  throw(); //!< find releasing
        };
        
    }
    
}


#endif
