
//! \file

#ifndef YACK_MEMORY_RAM_INCLUDED
#define YACK_MEMORY_RAM_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    extern uint64_t yack_ram; //!< total bytes in memory

    //! allocate *count * block_size bytes, then *count = bytes...
    void *          yack_ram_acquire(size_t *count,  const size_t block_size);
    void            yack_ram_release(void ** handle, size_t *count);


#if defined(__cplusplus)
}
#endif

#endif

