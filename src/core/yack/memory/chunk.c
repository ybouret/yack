
#include "yack/memory/chunk.h"

struct yack_memory_chunk_
{
    struct yack_memory_chunk_ *next;            //!< for list
    struct yack_memory_chunk_ *prev;            //!< for list
    uint8_t                   *data;            //!< first item
    const uint8_t             *last;            //!< first invalid item
    uint8_t                    first_available; //!< bookeeping
    uint8_t                    still_available; //!< bookeeping
    uint8_t                    provided_number; //!< initial count
};

size_t yack_memory_chunk_still_available(const yack_memory_chunk *chunk)
{
    assert(chunk);
    return chunk->still_available;
}


size_t yack_memory_chunk_provided_number(const yack_memory_chunk *chunk)
{
    assert(chunk);
    return chunk->provided_number;
}

size_t yack_memory_chunk_acquired_blocks(const yack_memory_chunk *chunk)
{
    assert(chunk);
    assert(chunk->still_available<=chunk->provided_number);
    return chunk->provided_number - chunk->still_available;
}

static uint8_t yack_memory_chunk_num_blocks(const size_t block_size, const size_t chunk_size)
{
    const size_t num_blocks = chunk_size/block_size;
    if(num_blocks>=0xff)
    {
        return 0xff;
    }
    else
    {
        return (uint8_t)(num_blocks&0xff);
    }
}

void yack_memory_chunk_initialize(yack_memory_chunk *chunk,
                                  const size_t       block_size,
                                  void *             chunk_data,
                                  const size_t       chunk_size)
{
    assert(chunk);
    assert(!(NULL==chunk_data&&0<chunk_size));
    assert(block_size>0);
    
    chunk->next            = NULL;
    chunk->prev            = NULL;
    chunk->first_available = 0;
    chunk->provided_number = yack_memory_chunk_num_blocks(block_size,chunk_size);
    chunk->still_available = chunk->provided_number;
    chunk->data            = chunk_data;
    chunk->last            = chunk_data + block_size * chunk->provided_number;
}




#include <string.h>

void *yack_memory_chunk_acquire(yack_memory_chunk *chunk,
                                const size_t       block_size)
{
    // sanity check
    assert(chunk->still_available>0);
    assert(chunk->still_available<=chunk->provided_number);
    assert(block_size>0);

    // return object
    {
        uint8_t *p = &chunk->data[ chunk->first_available*block_size ]; // get address
        chunk->first_available = *p;                                    // read next available address
        chunk->still_available--;                                       // bookeeping
        memset(p,0,block_size);                                         // zero memory
        return p;                                                       // done
    }
}

int    yack_memory_chunk_owns(const yack_memory_chunk *chunk,
                              const void              *block_addr,
                              const size_t             block_size)
{
    const uint8_t *data = (const uint8_t *)block_addr;
    assert(NULL!=chunk);
    if(data>=chunk->data&&data<chunk->last)
    {
        const size_t delta = (size_t)(data-chunk->data);
        if(0==(delta%block_size))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

int yack_memory_chunk_is_free(const yack_memory_chunk *chunk)
{
    assert(chunk);
    return chunk->still_available>=chunk->provided_number;
}

void yack_memory_chunk_release(yack_memory_chunk *chunk,
                               void              *block_addr,
                               const size_t       block_size)
{
    assert(NULL!=chunk);
    assert(NULL!=block_addr);
    assert(block_size>0);
    assert(yack_memory_chunk_owns(chunk,block_addr,block_size));

    {
        uint8_t     *to_release = (uint8_t *)block_addr;
        const size_t indx       = ((size_t)(to_release-chunk->data))/block_size;  assert(indx<(size_t)(chunk->provided_number));
        *to_release             = chunk->first_available;
        chunk->first_available  = (uint8_t)indx;
        chunk->still_available++;
    }
}

size_t  yack_memory_chunk_sizeof()
{
    return sizeof(struct yack_memory_chunk_);
}
