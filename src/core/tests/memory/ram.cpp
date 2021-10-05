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

YACK_UTEST(ram)
{
    uprng        ran;
    block        blocks[256];
    const size_t nblock = sizeof(blocks)/sizeof(blocks[0]);
    memory::ram  mem;
    const uint64_t ini = mem.get();
    
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
    YACK_CHECK(mem.get()==ini);

    for(size_t n=0;n<=10000;n+=100+ran.leq(100))
    {
        std::cerr << "Query #items=" << n << std::endl;
        test_for<uint32_t>(mem,n);
        test_for<block>(mem,n);
    }
    YACK_CHECK(mem.get()==ini);



}
YACK_UDONE()

