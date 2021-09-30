//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif



    typedef struct yack_memory_chunk_ yack_memory_chunk;
    void    yack_memory_chunk_initialize(yack_memory_chunk *chunk, const size_t block_size, void * chunk_data, const size_t chunk_size);
    size_t  yack_memory_chunk_still_available(const yack_memory_chunk *chunk);
    size_t  yack_memory_chunk_provided_number(const yack_memory_chunk *chunk);
    size_t  yack_memory_chunk_acquired_blocks(const yack_memory_chunk *chunk);
    int     yack_memory_chunk_owns(const yack_memory_chunk *chunk, const void *block_addr, const size_t block_size);
    int     yack_memory_chunk_is_free(const yack_memory_chunk *chunk);
    void  * yack_memory_chunk_acquire(yack_memory_chunk *chunk, const size_t block_size);
    void    yack_memory_chunk_release(yack_memory_chunk *chunk, void *block_addr, const size_t block_size);
    size_t  yack_memory_chunk_sizeof();




#if defined(__cplusplus)
}
#endif

#endif

