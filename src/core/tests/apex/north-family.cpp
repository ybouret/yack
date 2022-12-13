#include "yack/apex/north/qfamily.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"

using namespace yack;

YACK_UTEST(apex_north_family)
{
    YACK_SIZEOF(north::qfamily);
    
    matrix<int> vec(3,3);
    vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
    vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
    vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;
    
    vector<size_t>   rindx(3);
    north::qidx_bptr idxIO = new north::qidx_bank();
    rindx.ld_incr(1);
    rolling::down(rindx);
    std::cerr << "rindx=" << rindx << std::endl;
    std::cerr << "vbase=" << vec << std::endl;
    north::qfamily f(rindx,vec,idxIO);
}
YACK_UDONE()

