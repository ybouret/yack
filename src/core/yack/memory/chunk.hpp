//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! basic memory chunk
        //
        //______________________________________________________________________
        class chunk
        {
        public:
            static const size_t header; //!< aligned size of chunk
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
          
            //! setup
            chunk(const size_t block_size,
                  void        *chunk_data,
                  const size_t chunk_size) throw();

            //! cleanup
            ~chunk() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool is_empty()                                      const throw(); //!< still_available >= provided_number
            bool owns(const void *addr, const size_t block_size) const throw(); //!< in range and aligned

           
            void *acquire(const size_t block_size) throw();             //!< needs still_available>0
            void  release(void *addr, const size_t block_size) throw(); //!< previuosly acquired block
            
            //! compute an optimized, power of two memory area to hold chunk+data
            static size_t optimized_bytes_for(const size_t block_size, size_t &blocks_per_chunk) throw();
            
            static chunk *ram_create(const size_t block_size, const size_t full_bytes); //!< full_bytes should be optimied
            static void   ram_delete(chunk *,size_t full_bytes) throw();                //!< previously ram_created chunk

            uint8_t                    first_available; //!< bookeeping
            uint8_t                    still_available; //!< bookeeping
            const uint8_t              provided_number; //!< initial count
            uint8_t                    priv;            //!< unused
            uint8_t                   *data;            //!< first item
            const uint8_t * const      last;            //!< first invalid item
            chunk                     *next;            //!< for arena
            chunk                     *prev;            //!< for arena

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chunk);


        };
        
    }

}

#endif

