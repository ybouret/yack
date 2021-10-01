#include "yack/memory/ram.h"
#include "yack/memory/chunk.h"
#include "yack/system/exception.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
    };
}

YACK_UTEST(memory_chunk)
{
    uprng ran;
#if 0
    block blocks[256];
    {
        size_t             data[16];
        yack_memory_chunk *ch = static_cast<yack_memory_chunk *>(yack_out_of_reach_address(data));

        std::cerr << "Local Chunks..." << std::endl;

        for(size_t block_size=1;block_size<=32;++block_size)
        {
            std::cerr << "block_size=" << block_size << std::endl;
            for(size_t num_blocks=0;num_blocks<=300;++num_blocks)
            {
                size_t chunk_size = num_blocks;
                void  *chunk_data = yack_ram_acquire(&chunk_size,block_size);
                yack_memory_chunk_initialize(ch,block_size,chunk_data,chunk_size);

                const size_t count = yack_memory_chunk_provided_number(ch);
                for(size_t i=0;i<count;++i)
                {
                    blocks[i].addr = yack_memory_chunk_acquire(ch,block_size);
                    blocks[i].size = block_size;
                }
                ran.shuffle(blocks,count);
                for(size_t i=0;i<count/2;++i)
                {
                    yack_memory_chunk_release(ch,blocks[i].addr,block_size);
                }
                for(size_t i=0;i<count/2;++i)
                {
                    blocks[i].addr = yack_memory_chunk_acquire(ch,block_size);
                }
                ran.shuffle(blocks,count);
                for(size_t i=0;i<count;++i)
                {
                    yack_memory_chunk_release(ch,blocks[i].addr,block_size);
                }
                YACK_ASSERT(yack_memory_chunk_is_free(ch));

                yack_ram_release(&chunk_data,&chunk_size);
            }
        }
        YACK_CHECK(yack_ram_get()==0);
    }


    {
        std::cerr << std::endl;
        std::cerr << "Global Chunks..." << std::endl;

        for(size_t block_size=1;block_size<=256;++block_size)
        {
            const size_t       bytes = yack_memory_chunk_optimized_bytes(block_size);
            yack_memory_chunk *chunk = yack_memory_chunk_create(block_size,bytes);
            if(!chunk) throw libc::exception(ENOMEM,"yack_memory_chunk_create(%u)", unsigned(bytes) );

            std::cerr << "block_size=" << block_size << " => bytes=" << bytes << " => #blocks=" << yack_memory_chunk_provided_number(chunk) << std::endl;

            yack_memory_chunk_delete(chunk);
        }

        YACK_CHECK(yack_ram_get()==0);
    }

#endif

}
YACK_UDONE()

