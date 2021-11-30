
#include "yack/math/algebra/jacobi.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "yack/math/tao/v3.hpp"

using namespace yack;
using namespace math;

namespace {

    template <typename T> static inline
    void do_jacobi(randomized::bits &ran)
    {
        std::cerr << "jacobi<" << rtti::name<T>() << ">" << std::endl;
        jacobi<T> js(32);
        for(size_t n=1;n<=32;++n)
        {
            matrix<T>    a(n,n);
            matrix<T>    v(n,n);
            cxx_array<T> d(n);

            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=i;++j)
                {
                    a[i][j] = a[j][i] = ran.symm<T>();
                }
            }
            //std::cerr << "a=" << a << std::endl;
            if(js(a,d,v,sort_eigv_by_module))
            {
                //std::cerr << "d=diagm(" << d << ")" << std::endl;
                //std::cerr << "v=" << v << std::endl;
                matrix<T> vdv(n,n);
                {
                    matrix<T> vt(v,transposed);
                    matrix<T> dv(n,n);
                    tao::v3::dmul(dv,d,vt);
                    tao::v3::mmul(vdv,v,dv);
                }

                thin_array<T> lhs = a.get_contiguous();
                thin_array<T> rhs = vdv.get_contiguous();
                const T       rms = sqrt( tao::v1::mod2<T>::of(lhs,rhs) );
                std::cerr << "rms=" << rms << std::endl;

            }
            else
            {
                std::cerr << "couldn't find jacobi eigen" << std::endl;
            }
        }

        std::cerr << std::endl;
    }

}

YACK_UTEST(jacobi)
{
    randomized::rand_ ran;
    do_jacobi<float>(ran);
    do_jacobi<double>(ran);
    do_jacobi<long double>(ran);

}
YACK_UDONE()

