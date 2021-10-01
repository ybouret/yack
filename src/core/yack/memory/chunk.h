//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/system/setup.h"

#if 0

#if defined(__cplusplus)
extern "C" {
#endif


    //__________________________________________________________________________
    //
    //! memory chunk definition
    //__________________________________________________________________________
    typedef struct yack_memory_chunk_ yack_memory_chunk;

    //__________________________________________________________________________
    //
    //! memory chunk API: create/delete
    //__________________________________________________________________________
    yack_memory_chunk *yack_memory_chunk_create(const size_t block_size, size_t required_bytes); //!< create a chunk, with [LOCKED] RAM
    void               yack_memory_chunk_delete(yack_memory_chunk *chunk);                       //!< delete a chunk, with [LOCKED] RAM.
    size_t             yack_memory_chunk_optimized_bytes(const size_t block_size);               //!< required bytes for a block size

    //!initialize an allocated/static chunk
    void               yack_memory_chunk_initialize(yack_memory_chunk *chunk, const size_t block_size, void * chunk_data, const size_t chunk_size);

    //__________________________________________________________________________
    //
    //! memory chunk API: reading status
    //__________________________________________________________________________
    size_t  yack_memory_chunk_still_available(const yack_memory_chunk *chunk); //!< still available blocks
    size_t  yack_memory_chunk_provided_number(const yack_memory_chunk *chunk); //!< provided number of blocks
    size_t  yack_memory_chunk_acquired_blocks(const yack_memory_chunk *chunk); //!< currently allocated blocks
    int     yack_memory_chunk_is_free(const yack_memory_chunk *chunk);         //!< still available >= provided numbers

    //! check within range AND aligned
    int     yack_memory_chunk_owns(const yack_memory_chunk *chunk, const void *block_addr, const size_t block_size);

    //__________________________________________________________________________
    //
    //! memory chunk API: I/O
    //__________________________________________________________________________

    void  * yack_memory_chunk_acquire(yack_memory_chunk *chunk, const size_t block_size);                   //!< acquire a zeroed block size
    void    yack_memory_chunk_release(yack_memory_chunk *chunk, void *block_addr, const size_t block_size); //!< release a previously allocated block size







#if defined(__cplusplus)
}
#endif

#endif

#endif

