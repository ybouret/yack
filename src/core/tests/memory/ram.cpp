#include "yack/memory/ram.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
    };
}

YACK_UTEST(ram)
{
    uprng        ran;
    block        blocks[256];
    const size_t nblock = sizeof(blocks)/sizeof(blocks[0]);
    memory::ram  mem;

    for(size_t i=0;i<nblock;++i)
    {
        const size_t block_size = 1+ (rand() % 32);
        block       &blk = blocks[i];
        blk.size = 1 + (rand()%1024);
        blk.addr = mem.acquire(blk.size,block_size);
    }
    std::cerr << "crc: " << ucrc(blocks,nblock) << std::endl;
    ran.shuffle(blocks,nblock);
    std::cerr << "crc: " << ucrc(blocks,nblock) << std::endl;

    std::cerr << "ram: " << mem.get()<< std::endl;
    for(size_t i=0;i<nblock;++i)
    {
        block       &blk = blocks[i];
        mem.release(blk.addr,blk.size);
    }
    std::cerr << "ram: " << mem.get() << std::endl;
}
YACK_UDONE()

