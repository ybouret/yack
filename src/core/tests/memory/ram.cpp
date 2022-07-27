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

    template <typename T> static inline
    void test_for(memory::allocator &mem, size_t items)
    {

        size_t bytes = 0;
        T     *entry = mem.allocate<T>(items,bytes);
        std::cerr << "\t=> #items=" << items << ", #bytes=" << bytes << "  @" << entry << std::endl;
        mem.withdraw(entry,bytes);
        YACK_ASSERT(NULL==entry);
        YACK_ASSERT(0==bytes);
    }

}

YACK_UTEST(memory_ram)
{
    randomized::rand_  ran;
    block              blocks[256];
    const size_t       nblock = sizeof(blocks)/sizeof(blocks[0]);
    memory::ram        mem;
    const uint64_t     ini = memory::ram::allocated();
    
    for(size_t i=0;i<nblock;++i)
    {
        const size_t block_size = 1+ (rand() % 32);
        block       &blk = blocks[i];
        blk.size = 1 + (rand()%1024);
        blk.addr = mem.acquire(blk.size,block_size);
    }
    std::cerr << "crc: " << ucrc(blocks,nblock) << std::endl;
    randomized::shuffle::data(blocks,nblock,ran);
    std::cerr << "crc: " << ucrc(blocks,nblock) << std::endl;

    std::cerr << "ram: " << memory::ram::allocated() << std::endl;
    for(size_t i=0;i<nblock;++i)
    {
        block       &blk = blocks[i];
        mem.release(blk.addr,blk.size);
    }
    std::cerr << "ram: " << memory::ram::allocated() << std::endl;
    YACK_CHECK(memory::ram::allocated()==ini);

    for(size_t n=0;n<=10000;n+=100+ran.leq(100))
    {
        std::cerr << "Query #items=" << n << std::endl;
        test_for<uint32_t>(mem,n);
        test_for<block>(mem,n);
    }
    YACK_CHECK(memory::ram::allocated()==ini);



}
YACK_UDONE()

