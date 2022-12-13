#include "yack/apex/north/qbranch.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"

using namespace yack;

YACK_UTEST(apex_north_family)
{
    YACK_SIZEOF(north::qfamily);
    
    matrix<int> vec(4,3);
    vec[1][1] = 1;  vec[1][2] = 1;  vec[1][3] = 1;
    vec[2][1] = 0;  vec[2][2] = 1;  vec[2][3] = 0;
    vec[3][1] = 1;  vec[3][2] = 1;  vec[3][3] = 0;
    vec[4][1] = 0;  vec[4][2] = 0;  vec[4][3] = 1;

    vector<size_t>   rindx(4);
    north::qidx_bptr idxIO = new north::qidx_bank();
    rindx.ld_incr(1);
    std::cerr << "rindx=" << rindx << std::endl;
    std::cerr << "vbase=" << vec << std::endl;


    north::qbranch source;
    north::qbranch target;

    for(size_t i=1;i<=vec.rows;++i)
    {
        rolling::down(rindx);
        source.push_back( new north::qfamily(rindx,vec,idxIO) );
    }

    std::cerr << "source=" << source << std::endl;

    north::qfamily::generate(target,source,vec); YACK_ASSERT(0==source.size);
    std::cerr << "target=" << target << std::endl;

    target.swap_with(source);
    north::qfamily::generate(target,source,vec); YACK_ASSERT(0==source.size);
    std::cerr << "target=" << target << std::endl;




}
YACK_UDONE()

