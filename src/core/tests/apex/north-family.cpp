#include "yack/apex/north/qfamily.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"

using namespace yack;

YACK_UTEST(apex_north_family)
{
    YACK_SIZEOF(north::qfamily);
    
    matrix<int> vec(3,3);
    vec[1][1] = 1;  vec[1][2] = 1;  vec[1][3] = 1;
    vec[2][1] = 0;  vec[2][2] = 1;  vec[2][3] = 0;
    vec[3][1] = 1;  vec[3][2] = 1; vec[3][3] = 0;
    //vec[4][1] = 17; vec[4][2] = 0;  vec[4][3] = 19;

    vector<size_t>   rindx(3);
    north::qidx_bptr idxIO = new north::qidx_bank();
    rindx.ld_incr(1);
    rolling::down(rindx);
    std::cerr << "rindx=" << rindx << std::endl;
    std::cerr << "vbase=" << vec << std::endl;


    north::qfamily::list_type source;
    north::qfamily::list_type target;

    source.push_back( new north::qfamily(rindx,vec,idxIO) );

    std::cerr << "source=" << source << std::endl;

    north::qfamily::generate(target,source,vec);

    std::cerr << "target=" << target << std::endl;




}
YACK_UDONE()

