
#include "yack/math/algebra/svd.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "yack/math/tao/v3.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/math/numeric.hpp"

using namespace yack;
using namespace math;

namespace {

    template <typename T> static inline
    void do_svd(randomized::bits &ran)
    {
        std::cerr << "svd<" << rtti::name<T>() << ">" << std::endl;
        svd<T> SVD;
        for(size_t r=1;r<=8;++r)
        {
            for(size_t c=1;c<=r+r;++c)
            {
                matrix<T>    a(r,c);
                bring::fill(a,ran);
                matrix<T>    u(a);
                cxx_array<T> w(c);
                matrix<T>    v(c,c);

                if(!SVD.build(u,w,v))
                {
                    std::cerr << "[FAILURE]" << std::endl;
                }
                std::cerr << "w=" << w << " => ";
                const size_t ker = svd<T>::ker(w,1e-6);
                std::cerr << "w=" << w << "; ker=" << ker << std::endl;

                if(r==c)
                {
                    std::cerr << "\tsquare@" << r << "/ker=" << ker << std::endl;
                    if(ker<=0)
                    {
                        cxx_array<T> x(r);
                        cxx_array<T> b(r);
                        bring::fill(b,ran);
                        SVD.solve(u,w,v,x,b);
                        cxx_array<T> y(r);
                        a(y,x);
                        const T rms = sqrt(tao::v1::mod2<T>::of(b,y)/r);
                        std::cerr << "\trms=" << rms << std::endl;
                        //YACK_CHECK(rms<=numeric<T>::ftol);
                    }
                }

            }
        }

        std::cerr << std::endl;

    }

}

YACK_UTEST(svd)
{
    randomized::rand_ ran;

    do_svd<float>(ran);
    do_svd<double>(ran);
    do_svd<long double>(ran);

}
YACK_UDONE()

