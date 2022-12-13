#include "yack/apex/north/qbranch.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"

using namespace yack;

namespace {

    static inline
    void display_vec(const readable<apq> &q )
    {
        std::cerr << "\t" << q << std::endl;
    }

}

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

    std::cerr << "Loading source" << std::endl;
    for(size_t i=1;i<=vec.rows;++i)
    {
        rolling::down(rindx);
        source.boot(rindx,vec);
    }

    std::cerr << source << std::endl;

    while( source.next(vec) )
    {
        std::cerr << source << std::endl;
        source.for_each(display_vec);
    }

    


}
YACK_UDONE()
