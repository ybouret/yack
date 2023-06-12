
#include "yack/math/extended.hpp"
#include "yack/system/rtti.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

template <typename T>
static inline
void check1()
{
    std::cerr << std::endl;
    std::cerr << "check1<" << rtti::name<T>() << ">" << std::endl;
    static const T one(1);
    static const T half(0.5);
    const extended<T> xr1(one);
    YACK_CHECK(1==xr1.p);
    YACK_CHECK(std::abs(half-xr1.m) <= 0);
    std::cerr << "epsilon  = " << extended<T>::epsilon  << std::endl;
    std::cerr << "mant_dig = " << extended<T>::mant_dig << std::endl;
}

YACK_UTEST(math_extended)
{
    vector< extended<float> > xr;
    for(int i=1;i<argc;++i)
    {
        const float           r = ios::ascii::convert::real<float>(argv[i]);
        const extended<float> f = r;
        std::cerr << r << " => " << f << std::endl;
        xr << f;
    }
    std::cerr << xr << std::endl;

    {
        extended<float> prod = 1;
        for(size_t i=xr.size();i>0;--i) prod *= xr[i];
        std::cerr << "prod=" << prod << std::endl;
    }

    for(size_t i=1;i<=xr.size();++i)
    {
        const extended<float> &num = xr[i];
        for(size_t j=i;j<=xr.size();++j)
        {
            const extended<float> &den = xr[j];
            if(std::abs(den.m)<=0) continue;
            const extended<float> q = num / den;
            const extended<float> p = q * den;
            std::cerr << num << "/" << den << " = " << q << " => " << p << std::endl;
        }
    }

    std::cerr << std::endl << "sum" << std::endl;
    if(xr.size()>0)
    {
        extended<float> sum = xr[1];
        for(size_t i=2;i<=xr.size();++i)
        {
            sum += xr[i];
            std::cerr << "\t" << sum << " = " << *sum << std::endl;
        }
    }

    check1<float>();
    check1<double>();
    check1<long double>();

    std::cerr << std::endl << "sqrt" << std::endl;
    for(size_t i=1;i<=xr.size();++i)
    {
        const extended<float> xx = xr[i].abs();
        const extended<float> xs = xx.sqrt();
        std::cerr << "sqrt(" << *xx << ") = " << *xs << std::endl;
    }

    std::cerr << std::endl << "power" << std::endl;
    for(size_t i=1;i<=xr.size();++i)
    {
        for(unsigned n=0;n<=4;++n)
        {
            const extended<float> xx = xr[i];
            const extended<float> xp = xx.upower(n);
            std::cerr << "(" << *xx << ")^" << n << " = " << *xp << std::endl;
        }
    }


}
YACK_UDONE()

