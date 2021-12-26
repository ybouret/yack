
#include "yack/associative/addrbook.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

static const char s_chr[] = "hello";

YACK_UTEST(data_addrbook)
{
    addrbook ab;
    int      h_int[3] = { 0, 1, 2 };
    
    YACK_CHECK(ab.insert(h_int));
    YACK_CHECK(ab.insert(h_int+1));
    YACK_CHECK(ab.insert(h_int+2));
    
    YACK_CHECK(ab.insert(s_chr));
    YACK_CHECK(ab.insert(s_chr+1));
    YACK_CHECK(ab.insert(s_chr+2));

    (*ab).gv("addrbook.dot");
    ios::vizible::render("addrbook.dot");
    
    
}
YACK_UDONE()

