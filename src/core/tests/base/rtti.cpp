#include "yack/system/rtti.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;


#define YACK_RTTI(TYPE) std::cerr << "<" << #TYPE << "> => " << rtti::name<TYPE>() << std::endl

YACK_UTEST(rtti)
{


    rtti::display();

    //rtti::gv();
    YACK_RTTI(int);
    YACK_RTTI(apn);
    YACK_RTTI(apz);
    YACK_RTTI(apq);

}
YACK_UDONE()


