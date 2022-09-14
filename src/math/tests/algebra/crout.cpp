
#include "yack/type/complex.hpp"
#include "yack/apex.hpp"

#include "yack/math/algebra/crout.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/system/rtti.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/utils.hpp"

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
                        //std::cerr << "J=" << J << std::endl;
                        {

                            scalar_type extra = 0;
                            for(size_t i=n;i>0;--i)
                            {
                                for(size_t j=n;j>0;--j)
                                {
                                    if(i!=j)
                                    {
                                        extra += abs_of(J[i][j]);
                                    }
                                }
                            }
                            if(exact &&  extra>0)
                            {
                                throw exception("inexact inverse");
                            }
                        }

                        // TODO: test
                        
                        cr.adjoint(J,a0);
                        //std::cerr << "M=" << a0 << std::endl;
                        //std::cerr << "A=" << J << std::endl;
                        iota::mmul(I,a0,J);
                        //std::cerr << "dI=" << I << std::endl;

                        scalar_type extra = 0;
                        for(size_t i=n;i>0;--i)
                        {
                            for(size_t j=n;j>0;--j)
                            {
                                if(i!=j)
                                {
                                    extra += abs_of(I[i][j]);
                                }
                            }
                        }
                        if(exact &&  extra>0)
                        {
                            throw exception("inexact adjoint");
                        }

                        //std::cerr << "extra=" << extra << std::endl;
                        
                        
                        
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


    template <typename T> static inline
    void check_precise(const size_t      nmax,
                       randomized::bits &ran,
                       const bool        exact=false)
    {
        typedef typename crout<T>::scalar_type scalar_type;
        const string &who = rtti::name< typename crout<T>::type >();
        std::cerr << "check<" << who << ">" << std::endl;

        size_t outer = 8;
        if(who=="apq") outer=4;

        adder<T>      xadd;
        crout<T>      cr(nmax);
        for(size_t n=1;n<=nmax;++n)
        {
            matrix<T> a0(n,n);
            matrix<T> a1(n,n);
            matrix<T> a2(n,n);
            vector<T> r(n);
            vector<T> u1(n);
            vector<T> u2(n);
            vector<T> v1(n);
            vector<T> v2(n);
            matrix<T> I1(n,n);
            matrix<T> I2(n,n);

            matrix<T> J1(n,n);
            matrix<T> J2(n,n);
            matrix<T> adj(n,n);

            for(size_t cycle=1;cycle<=outer;++cycle)
            {
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        a0[i][j] = bring::get<T>(ran);
                    }
                }

                a1.assign(a0);
                a2.assign(a1);

                if(cr.build(a1))
                {
                    std::cerr << "// regular " << who << " " << n << "x" << n << " [";
                    YACK_ASSERT(cr.build(a2,xadd));
                    if(exact)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=n;++j)
                            {
                                YACK_ASSERT(abs_of(a1[i][j]-a2[i][j])<=0);
                            }
                        }
                    }

                    for(size_t iter=1;iter<=8;++iter)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            u1[i] = u2[i] = r[i] = bring::get<T>(ran);
                        }

                        cr.solve(a1,u1);
                        cr.solve(a2,u2,xadd);
                        {
                            const scalar_type du2 = iota::mod2<scalar_type>::of(u1,u2);
                            std::cerr << ":" << du2;
                            if(exact && du2>0 )
                            {
                                throw exception("mismatch solutions!!");
                            }
                        }

                        iota::mul(v1,a0,u1);
                        iota::mul(v2,a0,u2,xadd);
                        {
                            const scalar_type dv2 = iota::mod2<scalar_type>::of(v1,v2);
                            std::cerr << "/" << dv2;
                            if(exact && dv2>0 )
                            {
                                throw exception("mismatch retrieved!!");
                            }
                        }
                        {
                            const scalar_type dr1 = iota::mod2<scalar_type>::of(v1,r);
                            std::cerr << "/" << dr1;
                            if(exact && dr1>0 )
                            {
                                throw exception("mismatch retrieved 1!!");
                            }
                        }
                        {
                            const scalar_type dr2 = iota::mod2<scalar_type>::of(v2,r);
                            std::cerr << "/" << dr2;
                            if(exact && dr2>0 )
                            {
                                throw exception("mismatch retrieved 2!!");
                            }
                        }


                    }
                    std::cerr << ":]";

                    std::cerr << "/inv";
                    cr.inverse(a1,I1);
                    cr.inverse(a2,I2,xadd);
                    if(exact)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=n;++j)
                            {
                                YACK_ASSERT(abs_of(I1[i][j]-I2[i][j])<=0);
                            }
                        }
                    }

                    std::cerr << "/mul";
                    iota::mmul(J1,a0,I1);
                    iota::mmul(J2,a0,I2,xadd);

                    if(exact)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=n;++j)
                            {
                                YACK_ASSERT(abs_of(J1[i][j]-J2[i][j])<=0);
                                if(i!=j)
                                {
                                    YACK_ASSERT(abs_of(J2[i][j])<=0);
                                }
                                else
                                {
                                    YACK_ASSERT(abs_of(J2[i][j] - cr.t_one) <=0);
                                }
                            }
                        }
                    }

                    std::cerr << "/adj";
                    const T det = cr.determinant(a2);
                    cr.adjoint(adj,a0);

                    std::cerr << "/mul";
                    iota::mmul(J2,a0,adj);
                    for(size_t i=1;i<=n;++i)
                    {
                        for(size_t j=1;j<=n;++j)
                        {
                            J2[i][j]/=det;
                        }
                    }
                    //std::cerr << J2;
                    if(exact)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=n;++j)
                            {
                                if(i!=j)
                                {
                                    YACK_ASSERT(abs_of(J2[i][j])<=0);
                                }
                                else
                                {
                                    YACK_ASSERT(abs_of(J2[i][j] - cr.t_one) <=0);
                                }
                            }
                        }
                    }

                    std::cerr << std::endl;

                    

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

    if(true)
    {
        check_precise<float>(nmax,ran);
        check_precise<double>(nmax,ran);
        check_precise<long double>(nmax,ran);

        check_precise< complex<float> >(nmax,ran);
        check_precise< complex<double> >(nmax,ran);
        check_precise< complex<long double> >(nmax,ran);

        check_precise< apq >(nmax,ran,true);
    }

    if(false)
    {
        check_crout<float>(nmax,ran);
        check_crout<double>(nmax,ran);
        check_crout<long double>(nmax,ran);

        check_crout< complex<float> >(nmax,ran);
        check_crout< complex<double> >(nmax,ran);
        check_crout< complex<long double> >(nmax,ran);

        check_crout<apq>(nmax,ran,true);
    }

}
YACK_UDONE()



