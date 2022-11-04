#include "yack/type/integral.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(integral)
{
#define VALUE 17

    YACK_CHECK(VALUE == (integral::convert<uint8_t,uint8_t>(VALUE,NULL)) );
    YACK_CHECK(VALUE == (integral::convert<uint8_t,uint16_t>(VALUE,NULL)) );
    YACK_CHECK(VALUE == (integral::convert<uint8_t,uint32_t>(VALUE,NULL)) );
    YACK_CHECK(VALUE == (integral::convert<uint8_t,uint64_t>(VALUE,NULL)) );


}
YACK_UDONE()

