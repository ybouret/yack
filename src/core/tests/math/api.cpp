#include "yack/math/api.hpp"
#include "yack/utest/run.hpp"

#include "yack/type/complex.hpp"
#include <typeinfo>

using namespace yack;

#define SHOW_TYPEINFO(TYPE) std::cerr << std::setw(20) << #TYPE << " : " << typeid(TYPE).name() << std::endl;

YACK_UTEST(math_api)
{

    std::cerr << "---- unsigned ----" << std::endl;
    SHOW_TYPEINFO(uint8_t);
    SHOW_TYPEINFO(uint16_t);
    SHOW_TYPEINFO(uint32_t);
    SHOW_TYPEINFO(uint64_t);
    std::cerr << std::endl;

    SHOW_TYPEINFO(unsigned char)
    SHOW_TYPEINFO(unsigned short);
    SHOW_TYPEINFO(unsigned int);
    SHOW_TYPEINFO(unsigned long);
    SHOW_TYPEINFO(unsigned long long);
    SHOW_TYPEINFO(size_t);
    std::cerr << std::endl;

    std::cerr << "---- signed ----" << std::endl;
    SHOW_TYPEINFO(int8_t);
    SHOW_TYPEINFO(int16_t);
    SHOW_TYPEINFO(int32_t);
    SHOW_TYPEINFO(int64_t);
    std::cerr << std::endl;

    SHOW_TYPEINFO(char)
    SHOW_TYPEINFO(short);
    SHOW_TYPEINFO(int);
    SHOW_TYPEINFO(long);
    SHOW_TYPEINFO(long long);
    SHOW_TYPEINFO(ptrdiff_t);
    std::cerr << std::endl;



    {
        const complex<float> cplx(2,2);
        std::cerr << math::mod2_of(cplx) << std::endl;
        std::cerr << math::abs_of(cplx)  << std::endl;
    }
}
YACK_UDONE()


