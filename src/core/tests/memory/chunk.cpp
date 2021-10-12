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

    static inline void check_all_different(const block *blocks, const size_t count)
    {
        for(size_t i=0;i<count;++i)
        {
            const void *lhs = blocks[i].addr;
            for(size_t j=i+1;j<count;++j)
            {
                const void *rhs = blocks[j].addr;
                if(lhs==rhs)
                {
                    throw exception("same blocks[%u] and [%u]", unsigned(i), unsigned(j) );
                }
                YACK_ASSERT(lhs!=rhs);
            }
        }
    }

}

YACK_UTEST(memory_chunk)
{

    uprng          ran;
    block          blocks[256];
    memory::ram    mem;
    const uint64_t ini = memory::ram::allocated();

    std::cerr << "Local Chunks..." << std::endl;

    for(size_t block_size=1;block_size<=32;++block_size)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        for(size_t num_blocks=0;num_blocks<=300;++num_blocks)
        {
            size_t        chunk_size = num_blocks;
            void         *chunk_data = mem.acquire(chunk_size,block_size);
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
            check_all_different(blocks,count);
            for(size_t i=0;i<count;++i)
            {
                ch.release(blocks[i].addr,block_size);;
            }
            YACK_ASSERT(ch.operated_number<=0);

           mem.release(chunk_data,chunk_size);
        }
    }
    YACK_CHECK(memory::ram::allocated()==ini);

    std::cerr << std::endl;
    std::cerr << "Global Chunks..." << std::endl;

    for(size_t block_size=1;block_size<=256;++block_size)
    {
        size_t         blocks     = 0;
        const size_t   frame_size = memory::chunk::optimized_frame_size(block_size,blocks,false);
        memory::chunk *ch        = memory::chunk::create_frame(block_size,frame_size,mem);

        YACK_ASSERT(ch->provided_number==blocks);


        memory::chunk::delete_frame(ch,frame_size,mem);
    }

    YACK_CHECK(memory::ram::allocated()==ini);

    YACK_SIZEOF(memory::chunk);
    std::cerr << "chunk::header=" << memory::chunk::header << std::endl;
}
YACK_UDONE()

