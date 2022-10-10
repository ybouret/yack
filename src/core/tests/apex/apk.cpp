#include "yack/apex/kernel.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

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


}
YACK_UDONE()

