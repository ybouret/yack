
//! \file

#ifndef YACK_MEMORY_RAM_INCLUDED
#define YACK_MEMORY_RAM_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{

#define YACK_CHUNK_SIZE 4096 //!< default block size

    namespace memory
    {
        struct ram
        {
            static uint64_t get() throw(); //!< get the total, bookkept allocated ram byte

            //! allocate *count * block_size bytes, then set *count = bytes
            static void * acquire(size_t &count,  const size_t block_size);

            //! deallocate a previously acquired memory block
            static void   release(void * &addr, size_t &size) throw();

        };
    }


}


#endif

