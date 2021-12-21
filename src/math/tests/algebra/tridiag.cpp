#include "yack/apex.hpp"
#include "yack/math/algebra/tridiag.hpp"
#include "yack/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/system/rtti.hpp"
#include "yack/math/tao/v1.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void test_tri(randomized::bits &ran, const size_t max_dim, bool check=false)
    {
        std::cerr << "using <" << rtti::name<T>() << ">" << std::endl;
        for(size_t n=1;n<=max_dim;++n)
        {
            std::cerr << "\tdim=" << n << std::endl;
            math::tridiag<T> t(n);
            for(size_t iter=0;iter<8;++iter)
            {
                for(size_t i=n;i>0;--i)
                {
                    bring::fill(t.a,ran);
                    bring::fill(t.b,ran);
                    bring::fill(t.c,ran);
                }

                vector<T> r(n,0);
                vector<T> u(n,0);
                vector<T> q(n,0);

                for(size_t jter=0;jter<4;++jter)
                {
                    bring::fill(r,ran);
                    if(!t.solve(u,r))
                    {
                        std::cerr << "singular" << std::endl;
                        break;
                    }
                    t(q,u);

                    const typename scalar_for<T>::type m2 = math::tao::v1::mod2<T>::of(q,r);
                    std::cerr << "|" << m2;
                    if(check&&m2>0)
                    {
                        throw exception("tridiag failure");
                    }
                }

            }
            std::cerr << "|" << std::endl;
        }
        std::cerr << std::endl;

    }

}
YACK_UTEST(tridiag)
{
    randomized::rand_ ran;

    test_tri<float>(ran,16);
    test_tri<double>(ran,16);
    test_tri<long double>(ran,16);

    test_tri<complex<float>  >(ran,16);
    test_tri<complex<double> >(ran,16);
    test_tri<complex<long double> >(ran,16);

    test_tri<apq>(ran,6,true);

}
YACK_UDONE()



