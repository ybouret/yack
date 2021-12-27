
#include "yack/associative/addrbook.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

static const char s_chr[] = "hello";

YACK_UTEST(data_addrbook)
{
    addrbook ab;
    size_t      h_int[3] = { 0, 1, 2 };
    
    YACK_CHECK(ab.insert(h_int));
    YACK_CHECK(ab.insert(h_int+1));
    YACK_CHECK(ab.insert(h_int+2));
    
    YACK_CHECK(ab.insert(s_chr));
    YACK_CHECK(ab.insert(s_chr+1));
    YACK_CHECK(ab.insert(s_chr+2));

    ab.gv("addrbook.dot");
    ios::vizible::render("addrbook.dot");

    std::cerr << "List Of addresses:" << std::endl;
    size_t idx=0;
    for(addrbook::const_iterator it=ab.begin();it!=ab.end();++it,++idx)
    {
        const void *addr = *it;
        std::cerr << "\t@" << addr << std::endl;
        if(idx<3)
        {
            YACK_CHECK(idx==*(const size_t*)addr);
        }
        else
        {
            if(idx<6)
            {
                YACK_CHECK(s_chr[idx-3]==*(const char*)addr);
            }
        }
    }

    
}
YACK_UDONE()

