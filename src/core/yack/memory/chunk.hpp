//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/system/setup.hpp"

namespace yack
{

    namespace memory
    {

        class chunk
        {
        public:
            uint8_t                   *data;            //!< first item
            const uint8_t             *last;            //!< first invalid item
            uint8_t                    first_available; //!< bookeeping
            uint8_t                    still_available; //!< bookeeping
            const uint8_t              provided_number; //!< initial count
            chunk                     *next;
            chunk                     *prev;


            chunk(const size_t block_size,
                  void        *chunk_data,
                  const size_t chunk_size) throw();
            ~chunk() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chunk);


        }
    };

}

#endif

