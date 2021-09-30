//! \file

#ifndef YACK_MEMORY_CHUNK_INCLUDED
#define YACK_MEMORY_CHUNK_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    struct yack_memory_chunk
    {
        struct yack_memory_chunk *next;            //!< for list
        struct yack_memory_chunk *prev;            //!< for list
        uint8_t                  *data;            //!< first item
        const uint8_t            *last;            //!< first invalid item
        uint8_t                   first_available; //!< bookeeping
        uint8_t                   still_available; //!< bookeeping
        uint8_t                   provided_number; //!< initial count
    };

    void yack_memory_chunk_init(struct yack_memory_chunk *chunk,
                                const size_t               block_size,
                                void *                     chunk_data,
                                const size_t               chunk_size);


#if defined(__cplusplus)
}
#endif

#endif

