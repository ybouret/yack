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
    std::cerr << std::endl;

    std::cerr << "Unsigned/16" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint16_t, uint64_t>  :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    std::cerr << "Unsigned/32" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint8_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint32_t, uint64_t> :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    std::cerr << "Unsigned/64" << std::endl;
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint8_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint16_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint32_t> :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::u2u_integral<uint64_t, uint64_t> :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    WRONG( (core::u2u_integral<uint8_t,uint16_t>::convert(1000,"example")) );
    WRONG( (core::u2u_integral<uint8_t,uint32_t>::convert(1000,"example")) );
    WRONG( (core::u2u_integral<uint8_t,uint64_t>::convert(1000,"example")) );

    WRONG( (core::u2u_integral<uint16_t,uint32_t>::convert(100000,"example")) );
    WRONG( (core::u2u_integral<uint16_t,uint64_t>::convert(100000,"example")) );

    WRONG( (core::u2u_integral<uint32_t,uint64_t>::convert(YACK_U64(0xfffffffff),"example")) );
    std::cerr << std::endl;


    std::cerr << "Signed to Unsigned/8" << std::endl;
    YACK_CHECK(UVALUE == (core::s2u_integral<uint8_t,  int8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint8_t,  int16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint8_t,  int32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint8_t,  int64_t>  :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    std::cerr << "Signed to Unsigned/16" << std::endl;
    YACK_CHECK(UVALUE == (core::s2u_integral<uint16_t,  int8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint16_t,  int16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint16_t,  int32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint16_t,  int64_t>  :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    std::cerr << "Signed to Unsigned/32" << std::endl;
    YACK_CHECK(UVALUE == (core::s2u_integral<uint32_t,  int8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint32_t,  int16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint32_t,  int32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint32_t,  int64_t>  :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;

    std::cerr << "Signed to Unsigned/64" << std::endl;
    YACK_CHECK(UVALUE == (core::s2u_integral<uint64_t,  int8_t>   :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint64_t,  int16_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint64_t,  int32_t>  :: convert(UVALUE,NULL)) );
    YACK_CHECK(UVALUE == (core::s2u_integral<uint64_t,  int64_t>  :: convert(UVALUE,NULL)) );
    std::cerr << std::endl;


    YACK_CHECK( UVALUE == (i2i<unsigned char,char>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,unsigned char>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,short>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,unsigned short>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,int>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,unsigned>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,long>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,unsigned long>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,long long>(UVALUE,NULL)) );
    YACK_CHECK( UVALUE == (i2i<unsigned char,unsigned long long>(UVALUE,NULL)) );



}
YACK_UDONE()

