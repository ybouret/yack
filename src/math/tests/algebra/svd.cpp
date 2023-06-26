
#include "yack/math/algebra/svd.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/math/look-for.hpp"

using namespace yack;
using namespace math;

namespace {

    template <typename T> static inline
    void do_svd(randomized::bits &ran)
    {
        std::cerr << "svd<" << rtti::name<T>() << ">" << std::endl;
        svd<T> SVD;
        typedef extended<T> xtype;

        for(size_t r=1;r<=10;++r)
        {
            for(size_t c=1;c<=r+r;++c)
            {
                matrix<xtype>    a(r,c);
                bring::fill(a,ran);
                matrix<xtype>    u(a);
                cxx_array<xtype> w(c);
                matrix<xtype>    v(c,c);

                if(!SVD.build(u,w,v))
                {
                    std::cerr << "[FAILURE]" << std::endl;
                }
                //std::cerr << "w=" << w << " => ";
                const size_t nullity = look_for<xtype>::nullity::of(w,1e-6);
                //std::cerr << "w=" << w << "; ker=" << nullity << std::endl;

                if(r==c)
                {
                    std::cerr << "\tsquare@" << r << "/ker=" << nullity << std::endl;
                    if(nullity<=0)
                    {
                        cxx_array<xtype> x(r);
                        cxx_array<xtype> b(r);
                        bring::fill(b,ran);
                        SVD.solve(u,w,v,x,b);
                        cxx_array<xtype> y(r);
                        std::cerr << "a=" << a << std::endl;
                        //std::cerr << "u=" << u << std::endl;
                        //std::cerr << "w=diagm(" << w << ")" << std::endl;
                        //std::cerr << "v=" << v << std::endl;
                        std::cerr << "b=" << b << std::endl;
                        std::cerr << "x=" << x << std::endl;

                        a(y,x);

                        const T rms = *((SVD.add.mod2(b,y).reduce()/r).sqrt());
                        std::cerr << "\trms=" << rms << " / " << numeric<T>::ftol << std::endl;
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

