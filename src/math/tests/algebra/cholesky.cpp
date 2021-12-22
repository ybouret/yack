#include "yack/math/algebra/cholesky.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static inline void do_cholesky(randomized::bits &ran)
    {
        for(size_t n=1;n<=16;++n)
        {
            std::cerr << "rank=" << n << std::endl;
            matrix<T> U(n,n);
            matrix<T> a(n,n);
            matrix<T> a0(n,n);
            vector<T> b(n,0);
            vector<T> x(n,0);
            vector<T> r(n,0);
            math::cholesky<T> ch;

            for(size_t iter=0;iter<16;++iter)
            {
                // max rank U
                U.ld(0);
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=i;j<=n;++j)
                    {
                        U[i][j] = ran.symm<T>();
                    }
                }

                // max rank a
                math::tao::v3::gram(a,U);
                a0.assign(a);
                //std::cerr << "a =" << a << std::endl;
                //std::cerr << "a0=" << a0 << std::endl;
                if(!ch.build(a)) continue;

                for(size_t jter=0;jter<4;++jter)
                {
                    for(size_t i=n;i>0;--i)
                    {
                        b[i] = ran.symm<T>();
                    }

                    ch.solve(x,a,b);
                    a0(r,x);
                    const T rms = sqrt(math::tao::v1::mod2<T>::of(r,b)/n);
                    std::cerr << "|" << rms;
                }
            }

            std::cerr << std::endl;
        }

        std::cerr << std::endl;

    }
}
YACK_UTEST(cholesky)
{
    randomized::rand_ ran;
    do_cholesky<float>(ran);
    do_cholesky<double>(ran);
    do_cholesky<long double>(ran);

}
YACK_UDONE()

