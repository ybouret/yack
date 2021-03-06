
#include "yack/memory/large/book.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    struct block
    {
        void  *addr;
        size_t iln2;
    };
}

YACK_UTEST(memory_book)
{
    randomized::rand_  ran;

    std::cerr << "book.min_page_size=" << memory::book::min_page_size << std::endl;
    std::cerr << "book.max_page_size=" << memory::book::max_page_size << std::endl;
    std::cerr << "book.min_page_exp2 = " << memory::book::min_page_exp2 << std::endl;
    std::cerr << "book.max_page_exp2 = " << memory::book::max_page_exp2 << std::endl;
    std::cerr << "book.parts         = " << memory::book::parts         << std::endl;
    
    YACK_SIZEOF(memory::page);
    YACK_SIZEOF(memory::chapter);
    YACK_SIZEOF(memory::book);

    memory::book B;

    block        blocks[256];
    const size_t nblock = sizeof(blocks)/sizeof(blocks[0]);
    for(size_t i=0;i<nblock;++i)
    {
        block &b = blocks[i];
        b.iln2   = memory::book::min_page_exp2 + ran.leq(6);
        b.addr   = B.query(b.iln2);
        ran.fill(b.addr,size_t(1)<<b.iln2);
    }
    randomized::shuffle::data(blocks,nblock,ran);
    for(size_t i=0;i<nblock;++i)
    {
        block &b = blocks[i];
        B.store(b.addr,b.iln2);
    }

    B.display();

}
YACK_UDONE()

