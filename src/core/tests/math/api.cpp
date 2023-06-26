#include "yack/utest/run.hpp"
#include "yack/type/complex.hpp"
#include <typeinfo>
#include "../main.hpp"

using namespace yack;

#define SHOW_TYPEINFO(TYPE) std::cerr << std::setw(20) << #TYPE << " : " << typeid(TYPE).name() << std::endl;

namespace
{
    template <typename T> static inline
    void test_hypoth(randomized::bits &ran)
    {
        const T lo(-5);
        const T hi(5);
        const T del = hi-lo;
        {
            const T a = lo + del * bring::get<T>(ran);
            const T b = lo + del * bring::get<T>(ran);
            const T l = math::hypothenuse(a,b);
            std::cerr << "(";
            std::cerr << "(" << a << ")^2";
            std::cerr << "+";
            std::cerr << "(" << b << ")^2";
            std::cerr << ")^(0.5)=" << l << std::endl;

        }
    }
}

YACK_UTEST(math_api)
{
    randomized::rand_ ran;
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
        std::cerr << "on complex" << std::endl;
        const complex<float> cplx(3,2);
        std::cerr << math::mod2_of(cplx) << std::endl;
        std::cerr << math::abs_of(cplx)  << std::endl;
    }

    {
        test_hypoth<float>(ran);
        test_hypoth< extended<float> >(ran);

        
        test_hypoth<double>(ran);
        test_hypoth< extended<double> >(ran);

        test_hypoth<long double>(ran);
        test_hypoth< extended<long double> >(ran);

    }

}
YACK_UDONE()


