#include "yack/math/algebra/gauss-jordan.hpp"
#include "yack/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace math;


namespace
{
    template <typename T>
    static inline
    void do_test_gj(randomized::bits &ran)
    {
        for(size_t n=1;n<=3;++n)
        {
            matrix<T> a(n,n);
            vector<T> b(n,0);

            bring::fill(a,ran);
            bring::fill(b,ran);

            std::cerr << "a=" << a << std::endl;
            std::cerr << "b=" << b << std::endl;

            if( gauss_jordan<T>::build(a,b) )
            {
                std::cerr << "at=" << a << std::endl;
                std::cerr << "bt=" << b << std::endl;
            }
            else
            {
                std::cerr << "singular..." << std::endl;
            }

        }
    }
}

YACK_UTEST(gaussj)
{
    randomized::rand_ ran;

    do_test_gj<double>(ran);

}
YACK_UDONE()

