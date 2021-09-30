
#include "yack/type/ints.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(ints)
{
    YACK_CHECK(sizeof(unsigned_int<1>::type)==1);
    YACK_CHECK(sizeof(unsigned_int<2>::type)==2);
    YACK_CHECK(sizeof(unsigned_int<4>::type)==4);
    YACK_CHECK(sizeof(unsigned_int<8>::type)==8);

    YACK_CHECK(sizeof(signed_int<1>::type)==1);
    YACK_CHECK(sizeof(signed_int<2>::type)==2);
    YACK_CHECK(sizeof(signed_int<4>::type)==4);
    YACK_CHECK(sizeof(signed_int<8>::type)==8);

    

    YACK_CHECK(is_signed<int>::value);
    YACK_CHECK(!is_signed<unsigned>::value);
}
YACK_UDONE()

