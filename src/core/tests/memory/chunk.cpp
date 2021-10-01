#include "yack/memory/ram.hpp"
#include "yack/memory/chunk.hpp"
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
    block blocks[256];

    std::cerr << "Local Chunks..." << std::endl;

    for(size_t block_size=1;block_size<=32;++block_size)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        for(size_t num_blocks=0;num_blocks<=300;++num_blocks)
        {
            size_t        chunk_size = num_blocks;
            void         *chunk_data = memory::ram::acquire(chunk_size,block_size);
            memory::chunk ch(block_size,chunk_data,chunk_size);
            const size_t  count = ch.provided_number;
            for(size_t i=0;i<count;++i)
            {
                blocks[i].addr = ch.acquire(block_size);
                blocks[i].size = block_size;
            }
            ran.shuffle(blocks,count);
            for(size_t i=0;i<count/2;++i)
            {
                ch.release(blocks[i].addr,block_size);
            }
            for(size_t i=0;i<count/2;++i)
            {
                blocks[i].addr = ch.acquire(block_size);
            }
            ran.shuffle(blocks,count);
            for(size_t i=0;i<count;++i)
            {
                ch.release(blocks[i].addr,block_size);;
            }
            YACK_ASSERT(ch.is_empty());

            memory::ram::release(chunk_data,chunk_size);
        }
    }
    YACK_CHECK(memory::ram::get()==0);

    std::cerr << std::endl;
    std::cerr << "Global Chunks..." << std::endl;

    for(size_t block_size=1;block_size<=256;++block_size)
    {
        size_t         blocks = 0;
        const size_t   optims = memory::chunk::optimized_bytes_for(block_size,blocks);
        memory::chunk *ch     = memory::chunk::ram_create(block_size,optims);

        YACK_CHECK(ch->provided_number==blocks);


        memory::chunk::ram_delete(ch,optims);
    }

    YACK_CHECK(memory::ram::get()==0);


}
YACK_UDONE()

