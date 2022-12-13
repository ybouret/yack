#include "yack/apex/north/qbranch.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/roll.hpp"
#include "yack/apex/kernel.hpp"

using namespace yack;

namespace {

    static inline
    void display_vec(const readable<apq> &q )
    {
        std::cerr << "\t" << q << std::endl;
    }

    static inline void create_topo(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=1;i<=nu.rows;++i)
        {
            for(size_t j=1;j<=nu.cols;++j)
            {
                const double p = ran.to<double>();
                if(p<0.4) {
                    nu[i][j] = 0;
                }
                else
                {
                    nu[i][j] = static_cast<int>( ran.in(-2,2) );
                }
            }
        }
    }

    static inline void test_qb(north::qbranch &source,
                               const size_t    eqs,
                               const size_t    spc,
                               randomized::bits &ran)
    {

        source.prune();
        matrix<int> nu(eqs,spc);
        do
        {
            create_topo(nu,ran);
        } while( apk::rank_of(nu) < eqs );

        std::cerr << "nu=" << nu << std::endl;

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

    randomized::rand_ ran;
    for(size_t eqs=2; eqs<=2; ++eqs)
    {
        for(size_t spc=eqs;spc<=eqs+1;++spc)
        {
            test_qb(source,eqs,spc,ran);
        }
    }


}
YACK_UDONE()

