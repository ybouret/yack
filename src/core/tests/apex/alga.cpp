#include "yack/apex/alga.hpp"
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

        if( alga::are_prop(lhs,rhs) )
        {
            std::cerr << "\tproportional" << std::endl;
        }
        else
        {
            std::cerr << "\tnot proportional" << std::endl;
        }
    }

    template <typename T> static inline
    void test_prop( randomized::bits &ran )
    {
        std::cerr << "Test PropTo" << std::endl;
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

        std::cerr << std::endl;

    }
}



YACK_UTEST(alga)
{
    randomized::rand_ ran;

    {
        vector<apq> Q;
        Q << apq(10,3);
        Q << apq(-5,6);
        Q << apq(-20,3);
        std::cerr << "Q=" << Q << std::endl;
        {
            vector<apq> Q2(Q);
            alga::simplify(Q2);
            std::cerr << "Q=" << Q2 << std::endl;
        }
        {
            vector<apq> Q2(Q);
            alga::univocal(Q2);
            std::cerr << "Q=" << Q2 << std::endl;
        }
    }


    {
        vector<apz> Z;
        Z << -4;
        Z <<  2;
        Z << -8;
        Z << 22;
        std::cerr << "Z=" << Z << std::endl;
        {
            vector<apz> Z2(Z);
            alga::simplify(Z);
            std::cerr << "Z=" << Z << std::endl;
        }

        {
            vector<apz> Z2(Z);
            apn z2;
            alga::definite(Z,z2);
            std::cerr << "Z=" << Z << ", Z^2=" << z2 << std::endl;
        }
    }

    test_prop<apq>(ran);

    test_prop<apz>(ran);

    test_prop<int>(ran);


}
YACK_UDONE()

