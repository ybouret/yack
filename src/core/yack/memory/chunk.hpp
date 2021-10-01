//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace memory
    {

        class chunk
        {
        public:
            static const size_t header;

            chunk(const size_t block_size,
                  void        *chunk_data,
                  const size_t chunk_size) throw();

            ~chunk() throw();

            bool is_empty()         const throw();
            bool owns(const void *addr, const size_t block_size) const throw();

            //! still_available>0
            void *acquire(const size_t block_size) throw();

            //! previsuly acquired block
            void  release(void *addr, const size_t block_size) throw();

            static size_t optimized_bytes_for(const size_t block_size, size_t &blocks_per_chunk) throw();
            static chunk *create(const size_t block_size);

            uint8_t                    first_available; //!< bookeeping
            uint8_t                    still_available; //!< bookeeping
            const uint8_t              provided_number; //!< initial count
            uint8_t                    priv;            //!< unused
            uint8_t                   *data;            //!< first item
            const uint8_t * const      last;            //!< first invalid item
            chunk                     *next;
            chunk                     *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chunk);


        };
        
    }

}

#endif

