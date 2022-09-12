
#include "yack/type/complex.hpp"
#include "yack/apex.hpp"

#include "yack/math/algebra/crout.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/system/rtti.hpp"
#include "yack/sequence/vector.hpp"

#include "../../../core/tests/main.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void check_crout(const size_t      nmax,
                     randomized::bits &ran,
                     const bool exact = false)
    {
        typedef typename crout<T>::scalar_type scalar_type;
        adder<T>           xadd;
        adder<scalar_type> sadd;
        multiplier<T>      xmul;

        const string &who =rtti::name< typename crout<T>::type >();

        std::cerr << "type: " << who <<  " / scal: " << rtti::name< typename crout<T>::scalar_type >() << std::endl;

        size_t         inner = 16;
        size_t         count = 8;
        if(who=="apq")
        {
            count = 4;
            inner = 2;
        }

        for(size_t n=0;n<=nmax;++n)
        {
            crout<T> cr(n);
            std::cerr << "nmax=" << cr.nmax << ", granted=" << cr.granted() << std::endl;

            if(n>0)
            {
                matrix<T> a(n,n);
                matrix<T> a0(n,n);
                matrix<T> I(n,n);
                vector<T> r(n);
                vector<T> u(n);
                vector<T> v(n);
                matrix<T> J(n,n);
                
                for(size_t outer=count;outer>0;--outer)
                {
                    for(size_t i=1;i<=n;++i)
                    {
                        for(size_t j=1;j<=n;++j)
                        {
                            a[i][j] = bring::get<T>(ran);
                        }
                    }

                    a0.assign(a);


                    if(cr.build(a))
                    {
                        std::cerr << "// regular " << who << " " << n << "x" << n << " [";                        
                        for(size_t iter=inner;iter>0;--iter)
                        {
                            for(size_t i=n;i>0;--i)
                            {
                                r[i] = bring::get<T>(ran);
                            }
                            iota::load(u,r);
                            cr.solve(a,u);
                            iota::mul(v,a0,u,xadd);
                            //std::cerr << "r=" << r  << std::endl;
                            //std::cerr << "u=" << u  << std::endl;
                            //std::cerr << "v=" << v  << std::endl;
                            const scalar_type m2 = iota::mod2<scalar_type>::of(r,v,sadd);
                            std::cerr << ":" << m2;
                            if(exact && abs_of(m2) > 0)
                            {
                                throw exception("inexact result!!");
                            }
                        }
                        std::cerr << ":]" << std::endl;

                        cr.inverse(a,I);
                        
                        iota::mmul(J,a0,I);
                        std::cerr << "J=" << J << std::endl;
                        
                        // TODO: test
                        
                        cr.adjoint(J,a0);
                        std::cerr << "M=" << a0 << std::endl;
                        std::cerr << "A=" << J << std::endl;
                        iota::mmul(I,a0,J);
                        std::cerr << "dI=" << I << std::endl;

                        
                        
                        
                    }
                    else
                    {
                        std::cerr << "//singular" << std::endl;
                    }
                }
            }

        }

        std::cerr << std::endl;
    }
}

YACK_UTEST(crout)
{
    randomized::rand_ ran;

    size_t nmax = 5;
    if(argc>1)
    {
        nmax = ios::ascii::convert::to<size_t>(argv[1]);
    }


    check_crout<float>(nmax,ran);
    check_crout<double>(nmax,ran);
    check_crout<long double>(nmax,ran);

    check_crout< complex<float> >(nmax,ran);
    check_crout< complex<double> >(nmax,ran);
    check_crout< complex<long double> >(nmax,ran);

    check_crout<apq>(nmax,ran,true);

}
YACK_UDONE()



