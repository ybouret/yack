
#include "yack/math/xcomplex.hpp"
#include "yack/system/rtti.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;


YACK_UTEST(math_xcomplex)
{

    YACK_SIZEOF( yack::extended<float> );
    YACK_SIZEOF( yack::xcomplex<float> );
    YACK_SIZEOF( yack::extended<double> );
    YACK_SIZEOF( xcomplex<double> );
    YACK_SIZEOF( extended<long double> );
    YACK_SIZEOF( xcomplex<long double> );

    xcomplex<float> a;
    xcomplex<float> b = 1;
    xcomplex<float> c = complex<float>(2,3);

    std::cerr << a << b << c << std::endl;


    a *= 2;
    b  = c * 4;

}
YACK_UDONE()
