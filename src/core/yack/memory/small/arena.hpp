
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
        class dyadic;
        
        //______________________________________________________________________
        //
        //
        //! arena of chunks with same block size
        /**
         - acquire/release on demand of chunks
         - use power of two aligned chunk frames (to exchange with pages with frame_exp2)
         - an allocator must be provided for internal allocation of chunks
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
             \param bs         block_size for all the chunks
             \param dispatcher for frames
             \param compact    for space saving if only a few items shall be in use
             */
            arena(const size_t bs,
                  allocator   &dispatcher,
                  const bool   compact);
            ~arena() throw();               //!< cleanup

            //__________________________________________________________________
            //
            // memory methods
            //__________________________________________________________________
            
            //! acquire one zeroed block
            /**
             - try cache
             - look up around cache if available
             - create a new chunk if now more room
             - check tha abandoned!=acquiring
             */
            void *acquire();
            
            //! release an old block
            /**
             - use cache/look up
             - check up is abandonend
             - in case of two empty chunks, return the highest memory
             into reservoir
             */
            void  release(void *addr) throw();
            
            //! zero the block before releasing
            void  expunge(void *addr) throw();

            //__________________________________________________________________
            //
            // memory helpers
            //__________________________________________________________________

            //! create a zombie object
            template <typename T> inline
            T *zombie()
            {
                assert(sizeof(T)==block_size);
                return static_cast<T*>( acquire() );
            }
            
            //! acquire and default construct
            template <typename T> inline
            T *invoke() {
                assert(sizeof(T)==block_size);
                void *block = acquire();
                try        { return new(block) T(); }
                catch(...) { expunge(block); throw; }
            }

            //! destruct and expunge
            template <typename T> inline
            void revoke(T *args) throw()
            {
                assert(sizeof(T)==block_size);
                assert(NULL!=args);
                args->~T();
                expunge(args);
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   gc(dyadic &)   throw(); //!< garbage collector
            void   display_arena()  const; //!< current information
            size_t wasted() const throw(); //!< wasted bytes per chunk

        private:
            size_t       available;   //!< bookkeeping
            chunk       *acquiring;   //!< last acquiring
            chunk       *releasing;   //!< last releasing
            chunk       *abandoned;   //!< last empty chunk
            chunks_t     my_chunks;   //!< state for list of chunks
            ccache_t     reservoir;   //!< low-level reservoir of chunks
            allocator   &providing;   //!< allocator for frames
        
        public:
            arena       *next;         //!< for list
            arena       *prev;         //!< for list
            const size_t block_size;   //!< the same block size for all chunks
            const size_t new_blocks;   //!< for each chunk
            const size_t frame_size;   //!< a power of two
            const size_t frame_exp2;   //!< integer log 2
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(arena);
            void   grow();                //!< using reservoir or create new chunk
            void   kill(chunk *) throw(); //!< return memory
            void  *give()        throw(); //!< by acquiring
            void   take(void *)  throw(); //!< by releasing
            void   find(void *)  throw(); //!< find releasing
        };
        
    }
    
}


#endif
