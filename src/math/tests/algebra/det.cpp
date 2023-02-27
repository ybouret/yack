
#include "yack/type/complex.hpp"

#include "yack/math/algebra/lss.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"

#include "../../../core/tests/main.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void compute_determinant( randomized::bits &ran )
    {

        cameo::add<T> xadd;
        cameo::mul<T> xmul;

        for(size_t n=1;n<=10;++n)
        {
            matrix<T> A(n,n);
            crout<T>  cr(n);
            matrix<T> B(n,n);

            do {
                bring::fill(A,ran);
                B.assign(A);
            } while(!cr.build(B,xadd) );
            std::cerr << "A=" << A << std::endl;
            const T dA_ = cr.determinant(B);
            const T dA  = cr.determinant(B,xmul);
            std::cerr << "det(A)=" << dA << " / " << dA_ << std::endl;

        }
    }
}

YACK_UTEST(det)
{
    randomized::rand_ ran;

    compute_determinant<float>(ran);

}
YACK_UDONE()



