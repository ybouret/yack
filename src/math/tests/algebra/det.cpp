#include "yack/type/complex.hpp"
#include "yack/math/algebra/crout.hpp"
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

        for(size_t n=1;n<=8;++n)
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

            matrix<T> C0(n,n);
            cr.adjoint(C0,A);

            matrix<T> C1(n,n);
            cr.adjoint(C1,A,xadd,xmul);

            matrix<T> P0(n,n), P1(n,n);
            iota::mmul(P0,A,C0);
            iota::mmul(P1,A,C1,xadd);
            std::cerr << "P1=" << P1 << std::endl;
            std::cerr << std::endl;
        }
    }
}

YACK_UTEST(det)
{
    randomized::rand_ ran;

    compute_determinant<float>(ran);
    compute_determinant<double>(ran);
    compute_determinant<long double>(ran);


}
YACK_UDONE()



