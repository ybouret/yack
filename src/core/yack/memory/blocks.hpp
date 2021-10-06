
//! \file

#ifndef YACK_MEMORY_BLOCKS_INCLUDED
#define YACK_MEMORY_BLOCKS_INCLUDED 1

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
        class arena;

        //______________________________________________________________________
        //
        //
        //! handling blocks
        //
        //______________________________________________________________________
        class blocks
        {
        public:
            static const size_t minimal_capacity = 8; //!< memory for arenas
            static const char   designation[];        //!< memory blocks

            explicit blocks();          //!< setup with capacity but not arena
            virtual ~blocks() throw();  //!< cleanup

            void *acquire(const size_t block_size);                           //!< acquire block
            void  release(void *block_addr, const size_t block_size) throw(); //!< release block

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blocks);
            arena *data;      //!< first arena
            arena *last;      //!< first invalid arena
            arena *acquiring; //!< cache
            arena *releasing; //!< cache
            size_t size;      //!< 0<=size<=capacity
            size_t capacity;  //!< page_size/sizeof(arena)
            size_t page_size; //!< page size
            size_t page_exp2; //!< for page size

            void   create(const size_t block_size);
            void   update();
        };

    }

}

#endif
