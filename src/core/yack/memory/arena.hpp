
//! \file

#ifndef YACK_MEMORY_ARENA_INCLUDED
#define YACK_MEMORY_ARENA_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/data/core-list.hpp"
#include "yack/data/core-pool.hpp"

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
            typedef core_list_of<chunk> chunks_t; //!< legacy list for state
            typedef core_pool_of<chunk> ccache_t; //!< legacy pool for state

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
            void  display() const;             //!< current information

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
            size_t       available;   //!< bookkeeping
            chunk       *acquiring;   //!< last acquiring
            chunk       *releasing;   //!< last releasing
            chunk       *abandoned;   //!< last empty chunk
            chunks_t     io_chunks;   //!< state for list of chunks
            ccache_t     reservoir;   //!< low-level reservoir of chunks
            allocator   &providing;   //!< allocator for frames
        
        public:
            arena       *next;             //!< for list
            arena       *prev;             //!< for list
            const size_t chunk_block_size; //!< the same block size
            const size_t blocks_per_chunk; //!< for each chunk
            const size_t memory_per_chunk; //!< a power of two
            const size_t memory_signature; //!< integer log 2
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
            chunk *build();               //!< a new chunk
            void   grow();                //!< using reservoir or build()
            void   kill(chunk *) throw(); //!< return memory
            void  *give()        throw(); //!< by acquiring
            void   take(void *)  throw(); //!< by releasing
            void   find(void *)  throw(); //!< find releasing
        };
        
    }
    
}


#endif
