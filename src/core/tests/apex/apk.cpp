#include "yack/apex/kernel.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "../main.hpp"

using namespace yack;

namespace
{
    template <typename LHS, typename RHS> static inline
    void test_prop(LHS &lhs, RHS &rhs)
    {
        std::cerr << "check  " << lhs << std::endl;
        std::cerr << "versus " << rhs << std::endl;
        apq       fac;

        if( apk::are_prop(lhs,rhs,&fac) )
        {
            std::cerr << "\tprop with " << fac << std::endl;
        }
        else
        {
            std::cerr << "\tnot prop (" << fac << ")" << std::endl;
        }
    }

    template <typename T> static inline
    void test_prop( randomized::bits &ran )
    {
        const size_t n = 2 + ran.leq(4);

        vector<T> lhs(n), rhs(n);

        test_prop(lhs,rhs);

        lhs[1] = 1;
        test_prop(lhs,rhs);

        rhs[1] = 2;
        test_prop(lhs,rhs);

        rhs[1] = -2;
        test_prop(lhs,rhs);

        lhs[1] = -lhs[1];
        rhs[1] = -rhs[1];
        test_prop(lhs,rhs);

        lhs[2] = 4;
        test_prop(lhs,rhs);

        rhs[2] = 1;
        test_prop(lhs,rhs);

        rhs[2] = -8;
        test_prop(lhs,rhs);


    }
}

YACK_UTEST(apk)
{
    randomized::rand_ ran;

    YACK_SIZEOF(apn);
    YACK_SIZEOF(apz);
    YACK_SIZEOF(apq);


    vector<apq> data;

    for(size_t i=1;i<=4;++i)
    {
        data << apq(ran,8,8);
        std::cerr << "data=" << data << std::endl;
        const apn fac = apk::lcm(data);
        std::cerr << "|_factor=" << fac << std::endl;
        vector<apq> temp(data);
        for(size_t j=1;j<=temp.size();++j)
        {
            temp[j] *= fac;
        }
        std::cerr << "|_temp=" << temp << std::endl;
    }

    test_prop<int>(ran);

    for(size_t iter=0;iter<10;++iter)
    {
        data.make(10+ran.leq(10),0);
        for(size_t i=data.size();i>0;--i)
        {
            const unit_t c = ran.in(-1,1);
            switch( __sign::of(c) )
            {
                case __zero__: data[i] = 0; break;
                case positive:
                    data[i] = apq(1+ran.in(0,10),1+ran.in(0,20));
                    break;
                case negative:
                    data[i] = apq( -(1+ran.in(0,10)) , 1+ran.in(0,20) );
                    break;
            }
        }

        std::cerr << "data=" << data << std::endl;
        vector<apq> idat(data);
        apk::simplify(idat);
        std::cerr << "idat=" << idat << std::endl;


        apq rho = 0;
        for(size_t i=data.size();i>0;--i)
        {
            const apq &lhs = data[i];
            const apq &rhs = idat[i];

            if(0==lhs)
            {
                YACK_ASSERT(0==rhs);
            }
            else
            {
                YACK_ASSERT(0!=rhs);
                const apq ratio = rhs/lhs;
                if(0==rho)
                {
                    rho = ratio;
                }
                else
                {
                    YACK_ASSERT(ratio==rho);
                }
            }
        }
        std::cerr << "\trho=" << rho << std::endl;

    }


    for(size_t iter=0;iter<10;++iter)
    {
        const size_t n = 1+ran.leq(5);
        vector<apz>  z(n);
        for(size_t i=n;i>0;--i)
        {
            if( ran.choice() )
            {
                const apz tmp(ran,4);
                z[i] = tmp;
            }
        }

        std::cerr << "z=" << z;
        apk::simplify(z);
        std::cerr << " -> " << z << std::endl;
    }
    
    for(size_t iter=0;iter<10;++iter)
    {
        const size_t n = 1+ran.leq(5);
        vector<apn>  z(n);
        for(size_t i=n;i>0;--i)
        {
            if( ran.choice() )
            {
                const apn tmp(ran,4);
                z[i] = tmp;
            }
        }
        
        std::cerr << "n=" << z;
        apk::simplify(z);
        std::cerr << " -> " << z << std::endl;
    }


    for(size_t iter=0;iter<10;++iter)
    {
        matrix<int> M(1+ran.leq(5),1+ran.leq(5));
        bring::fill(M,ran);
        std::cerr << "rank(" << M << ")=" << apk::rank_of(M) << std::endl;
    }

}
YACK_UDONE()

