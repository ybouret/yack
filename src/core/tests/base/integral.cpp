#include "yack/type/integral.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

#define WRONG(expr) try { \
(void) (expr); } \
catch(const exception &e) {\
e.show(std::cerr);\
}

YACK_UTEST(integral)
{
#define UVALUE 127
    std::cerr << "Unsigned/8" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint8_t,  uint8_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint8_t,  uint16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint8_t,  uint32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint8_t,  uint64_t>  :: convert(UVALUE,NULL)) );


    std::cerr << "Unsigned/16" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint64_t>  :: convert(UVALUE,NULL)) );

    std::cerr << "Unsigned/32" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint8_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint64_t> :: convert(UVALUE,NULL)) );

    std::cerr << "Unsigned/64" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint8_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint64_t> :: convert(UVALUE,NULL)) );

    WRONG( (core::u2u_integral<uint8_t,uint16_t>::convert(1000,"example")) );
    WRONG( (core::u2u_integral<uint8_t,uint32_t>::convert(1000,"example")) );
    WRONG( (core::u2u_integral<uint8_t,uint64_t>::convert(1000,"example")) );

    WRONG( (core::u2u_integral<uint16_t,uint32_t>::convert(100000,"example")) );
    WRONG( (core::u2u_integral<uint16_t,uint64_t>::convert(100000,"example")) );

    WRONG( (core::u2u_integral<uint32_t,uint64_t>::convert(YACK_U64(0xfffffffff),"example")) );


}
YACK_UDONE()

