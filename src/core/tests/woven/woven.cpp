#include "yack/woven/qfamily.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"

using namespace yack;

YACK_UTEST(woven)
{


    vector<int>    v1; v1 << -1 << 2 << 0 << 0;
    std::cerr << "v1=" << v1 << std::endl;
    woven::indxFund qfund = new woven::indxBank();
    woven::qfamily F(v1.size(),3,qfund);
    YACK_CHECK(F.try_grow(v1,2));
    std::cerr << "F=" << F << " | " << *F.base << std::endl;

    cxx_array<short> v2(F.dimensions);

    v2[1] = 7;
    v2[2] = 13;
    std::cerr << "v2=" << v2 << std::endl;

    YACK_CHECK(F.try_grow(v2,3));
    std::cerr << "F=" << F << " | " << *F.base << std::endl;

    cxx_array<short> v3(F.dimensions);

    v3[1] =  0;
    v3[2] =  5;
    v3[3] = -3;
    std::cerr << "v3=" << v3 << std::endl;

    YACK_CHECK(F.try_grow(v3,1));
    std::cerr << "F=" << F << " | " << *F.base << std::endl;


    YACK_SIZEOF(woven::qvector);
    YACK_SIZEOF(woven::qfamily);

}
YACK_UDONE()

