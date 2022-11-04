#include "yack/type/integral.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(integral)
{
#define UVALUE 127
    YACK_CHECK(UVALUE == (core::uu_integral<uint8_t,  uint8_t>  :: convert(UVALUE,NULL)) );

    YACK_CHECK(UVALUE == (core::uu_integral<uint16_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint16_t, uint8_t>  :: convert(UVALUE,NULL)) );

    YACK_CHECK(UVALUE == (core::uu_integral<uint32_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint32_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint32_t, uint8_t> :: convert(UVALUE,NULL)) );

    YACK_CHECK(UVALUE == (core::uu_integral<uint64_t, uint64_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint64_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint64_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::uu_integral<uint64_t, uint8_t> :: convert(UVALUE,NULL)) );

}
YACK_UDONE()

