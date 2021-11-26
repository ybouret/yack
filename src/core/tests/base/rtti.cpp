#include "yack/system/rtti.hpp"
#include "yack/associative/be-addr.hpp"

#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/concurrent/single.hpp"

using namespace yack;


#define YACK_RTTI(TYPE) std::cerr << "<" << #TYPE << "> => " << rtti::name<TYPE>() << std::endl

YACK_UTEST(rtti)
{
    concurrent::single::verbose = true;

    rtti::display();

    //rtti::gv();
    YACK_RTTI(int);
    YACK_RTTI(apn);
    YACK_RTTI(apz);
    YACK_RTTI(apq);
    
    {
        int a = 10;
        be_address A(a);
        std::cerr << "a@" << (void*)&a << " => " << A  << std::endl;
    }

    {

    }

}
YACK_UDONE()


