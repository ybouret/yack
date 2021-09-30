
//! \file

#ifndef YACK_MEMORY_RAM_INCLUDED
#define YACK_MEMORY_RAM_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif


#define YACK_CHUNK_SIZE 4096 //!< default block size

    //! allocate *count * block_size bytes, then set *count = bytes
    void *          yack_ram_acquire(size_t *count,  const size_t block_size);

    //! deallocate a previously acquired memory block
    void            yack_ram_release(void ** handle, size_t *count);

    //! get the total, bookkept allocated ram byte
    uint64_t        yack_ram_get();


#if defined(__cplusplus)
}
#endif

#endif

