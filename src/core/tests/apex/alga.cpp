#include "yack/apex/alga.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(alga)
{
    vector<apq> Q;
    Q << apq(10,3);
    Q << apq(-5,6);
    Q << apq(20,3);
    std::cerr << "Q=" << Q << std::endl;
    vector<apq> Q2(Q);
    alga::simplify(Q2);
    std::cerr << "Q=" << Q2 << std::endl;

}
YACK_UDONE()

