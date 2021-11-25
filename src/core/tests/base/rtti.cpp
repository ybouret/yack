#include "yack/system/rtti.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;


#define YACK_RTTI(TYPE) std::cerr << "<" << #TYPE << "> => " << rtti::name<TYPE>() << std::endl

YACK_UTEST(rtti)
{


    rtti::display();

    //rtti::gv();
    YACK_RTTI(int);

}
YACK_UDONE()


