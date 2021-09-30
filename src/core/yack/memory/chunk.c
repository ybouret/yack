
#include "yack/memory/chunk.h"


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

void yack_memory_chunk_init(struct yack_memory_chunk *chunk,
                            const size_t              block_size,
                            void *                    chunk_data,
                            const size_t              chunk_size)
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
