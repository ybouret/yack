
#include "yack/type/complex.hpp"
#include "yack/apex.hpp"

#include "yack/math/algebra/crout.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/system/rtti.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void check_crout(const size_t nmax)
    {
        std::cerr << "type: " << rtti::name< typename crout<T>::type >() <<  " / scal: " << rtti::name< typename crout<T>::scalar_type >() << std::endl;
        for(size_t n=0;n<=nmax;++n)
        {
            crout<T> cr(n);
            std::cerr << "nmax=" << cr.nmax << ", granted=" << cr.granted() << std::endl;
        }

        std::cerr << std::endl;
    }
}

YACK_UTEST(crout)
{
    size_t nmax = 5;
    if(argc>1)
    {
        nmax = ios::ascii::convert::to<size_t>(argv[1]);
    }


    check_crout<float>(nmax);
    check_crout<double>(nmax);
    check_crout<long double>(nmax);

    check_crout< complex<float> >(nmax);
    check_crout< complex<double> >(nmax);
    check_crout< complex<long double> >(nmax);

    check_crout<apq>(nmax);

}
YACK_UDONE()



