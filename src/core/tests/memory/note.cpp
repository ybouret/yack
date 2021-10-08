

#include "yack/memory/note.hpp"
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

YACK_UTEST(memory_note)
{
    uprng ran;

    std::cerr << "note::max_page_size=" << memory::note::max_page_size << std::endl;
    std::cerr << "note::max_page_exp2=" << memory::note::max_page_exp2 << std::endl;
    std::cerr << "sizeof(note)       =" << sizeof(memory::note)        << std::endl;

    memory::note lw;
    lw.display();
}
YACK_UDONE()

