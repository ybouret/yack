#include "yack/type/constant.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/apex.hpp"

using namespace yack;

#define  SHOW(FIELD) std::cerr << #FIELD << " = " << *FIELD << std::endl

YACK_UTEST(type_constant)
{
    YACK_SIZEOF(const_field<short>);
    YACK_SIZEOF(const_field<double>);
    YACK_SIZEOF(const_field<string>);


    const_field<int>    iz;
    const_field<double> dz;
    const_field<string> sz;

    SHOW(iz);
    SHOW(dz);
    SHOW(sz);

    const_field<string> hello(transmogrify,"hello");
    std::cerr << *hello << std::endl;

}
YACK_UDONE()

