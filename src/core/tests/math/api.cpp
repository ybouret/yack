#include "yack/math/api.hpp"
#include "yack/utest/run.hpp"

#include "yack/type/complex.hpp"

using namespace yack;

YACK_UTEST(math_api)
{
    {
        const complex<float> cplx(2,2);
        std::cerr << math::mod2(cplx) << std::endl;
    }
}
YACK_UDONE()


