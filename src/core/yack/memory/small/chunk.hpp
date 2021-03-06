//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/memory/ownership.hpp"

namespace yack
{

    namespace memory
    {

        class allocator;

        //______________________________________________________________________
        //
        //
        //! basic memory chunk
        /**
         - no implicit allocator, all is set manually
         - an allocator must be provided to create/delete a frame
         */
        //
        //______________________________________________________________________
        class chunk
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const size_t header;                   //!< sizeof(chunk)
            static const size_t minimum_frame_blocks = 8; //!< when a frame is allocated

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
          
            //! setup and format blocks
            /**
             \param block_size > 0
             \param chunk_data user's memory entry
             \param chunk_size user's memory size
             */
            chunk(const size_t block_size,
                  void        *chunk_data,
                  const size_t chunk_size) throw();

            //! cleanup
            ~chunk() throw();

            //__________________________________________________________________
            //
            // query methods
            //__________________________________________________________________
            bool      contains(const void *addr)                      const throw(); //!< in range
            bool      owns(const void *addr, const size_t block_size) const throw(); //!< in range AND aligned
            ownership whose(const void *addr)                         const throw(); //!< query ownsership

            //__________________________________________________________________
            //
            // memory methods
            //__________________________________________________________________

            //! need still_available>0: get the first available block, zeroed
            void *acquire(const size_t block_size) throw();

            //! release a previously acquired block, untouched
            bool  release(void *addr, const size_t block_size) throw();

            //__________________________________________________________________
            //
            // using chunks as frames
            //__________________________________________________________________
            
            //! compute an optimized, power of two memory area to hold chunk+data
            /**
             \param block_size     is for the chunk
             \param blocks         are available per chunk
             \param compact        false=>any memory, true=>close to YACK_CHUNK_SIZE
             */
            static size_t optimized_frame_size(const size_t block_size,
                                               size_t      &blocks,
                                               const bool   compact) throw();

            //! create a single frame with chunk+data
            static chunk *create_frame(const size_t block_size,
                                       const size_t frame_size,
                                       allocator   &dispatcher);

            //! delete a previously created frame chunk
            static void   delete_frame(chunk     *used_chunk,
                                       size_t     frame_size,
                                       allocator &dispatcher) throw();
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            uint8_t                    first_available; //!< bookkeeping
            uint8_t                    still_available; //!< bookkeeping
            uint8_t                    operated_number; //!< bookkeeping
            const uint8_t              provided_number; //!< initial count : still_available + operated_number
            uint8_t                   *data;            //!< first item
            const uint8_t * const      last;            //!< first invalid item
            chunk                     *next;            //!< for arena
            chunk                     *prev;            //!< for arena

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chunk);
            void format(const size_t block_size) throw();

        };
        
    }

}

#endif

