
#include "yack/math/fcn/jacobian.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/type/utils.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/system/rtti.hpp"

using namespace yack;
using namespace math;

namespace
{

    template <typename T>
    struct Func
    {
        size_t count;

        template <typename OUTPUT,typename INPUT>
        inline void operator()(OUTPUT &y, INPUT &x)
        {
            for(size_t j=y.size();j>0;--j)
            {
                T ans = T(j);
                for(size_t i=x.size();i>0;--i)
                {
                    ans += ipower(x[i],j);
                }
                y[j] = ans;
            }
        }

    };

    template <typename T> static inline
    void test_fjac(randomized::bits &ran)
    {
        std::cerr << "jacobian for <" << rtti::name<T>() << ">" << std::endl;
        Func<T>     F = { 0 };
        jacobian<T> fjac(8);
        for(size_t domain=1;domain<=4;++domain)
        {
            vector<T> x(domain,0);
            std::cerr << "domain=" << domain << std::endl;
            for(size_t image=1;image<=4;++image)
            {
                std::cerr << "    image=" << image << std::endl;
                matrix<T> J(image,domain);
                for(size_t iter=0;iter<4;++iter)
                {
                    bring::fill(x,ran);
                    fjac(J,F,x);
                    std::cerr << "x=" << x << std::endl;
                    std::cerr << "J=" << J << std::endl;
                }
            }
        }
        std::cerr << std::endl;
    }

}


YACK_UTEST(fjac)
{
    randomized::rand_ ran;

    test_fjac<float>(ran);
    test_fjac<double>(ran);
    test_fjac<long double>(ran);

    YACK_SIZEOF(jacobian<float>);
    YACK_SIZEOF(jacobian<double>);
    YACK_SIZEOF(jacobian<long double>);
    YACK_SIZEOF(arrays_of<float>);
    YACK_SIZEOF(arrays_of<double>);
    YACK_SIZEOF(arrays_of<long double>);


}
YACK_UDONE()

