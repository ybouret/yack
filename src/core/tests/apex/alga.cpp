#include "yack/apex/alga.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(alga)
{
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


}
YACK_UDONE()

