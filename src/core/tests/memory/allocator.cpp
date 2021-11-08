
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/legacy.hpp"

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
        std::cerr << "items=" << std::setw(6) << items << " * " << std::setw(2) << sizeof(T) << " => ";
        T     *entry = mem.allocate<T>(items,bytes);
        std::cerr << items << " / bytes=" << bytes << std::endl;
        mem.withdraw(entry,bytes);
        YACK_ASSERT(NULL==entry);
        YACK_ASSERT(0==bytes);
    }

    static inline void test_alloc( memory::allocator &mem)
    {
        std::cerr << "Testing " << mem.variety() << std::endl;
        randomized::rand_   ran;
        block               blocks[1024];
        const size_t        nblock = sizeof(blocks)/sizeof(blocks[0]);

        for(size_t i=0;i<nblock;++i)
        {
            const size_t block_size = 1+ (rand() % 32);
            block       &blk = blocks[i];
            blk.size = 1 + (rand()%1024);
            blk.addr = mem.acquire(blk.size,block_size);
            YACK_ASSERT( out_of_reach::is0(blk.addr,blk.size) );
        }
        randomized::shuffle::data(blocks,nblock,ran);

        for(size_t i=0;i<nblock;++i)
        {
            block       &blk = blocks[i];
            mem.release(blk.addr,blk.size);
        }

        for(size_t n=0;n<=10000;n+=100+ran.leq(1000))
        {
            test_for<uint32_t>(mem,n);
            test_for<block>(mem,n);
        }
    }

}

YACK_UTEST(memory_alloc)
{
    randomized::rand_ ran;

    concurrent::single::verbose = true;
    test_alloc(memory::global::instance());
    test_alloc(memory::dyadic::instance());
    test_alloc(memory::pooled::instance());

    std::cerr << std::endl;
    std::cerr << "Using Legacy" << std::endl;
    {
        void        *addr[12];
        const size_t num = sizeof(addr)/sizeof(addr[0]);
        for(size_t i=0;i<num;++i)
        {
            addr[i] = memory::legacy::acquire( ran.leq(100) );
        }

        randomized::shuffle::data(addr,num,ran);
        for(size_t i=0;i<num;++i)
        {
            memory::legacy::release(addr[i]);
            addr[i] = 0;
        }
    }


    std::cerr << std::endl;

}
YACK_UDONE()
