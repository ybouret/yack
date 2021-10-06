
#include "yack/memory/book.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{

}

YACK_UTEST(memory_book)
{
    std::cerr << "book.min_page_size=" << memory::book::min_page_size << std::endl;
    std::cerr << "book.max_page_size=" << memory::book::max_page_size << std::endl;
    std::cerr << std::endl;
    std::cerr << "book.min_page_exp2 = " << memory::book::min_page_exp2 << std::endl;
    std::cerr << "book.max_page_exp2 = " << memory::book::max_page_exp2 << std::endl;
    std::cerr << "book.parts         = " << memory::book::parts         << std::endl;
    std::cerr << "book.bytes         = " << memory::book::bytes         << std::endl;

    YACK_SIZEOF(memory::page);
    YACK_SIZEOF(memory::chapter);
    YACK_SIZEOF(memory::book);

}
YACK_UDONE()

