#include "yack/woven/qbranch.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/container/matrix.hpp"
#include "../main.hpp"
#include "yack/apex/flak.hpp"

using namespace yack;

YACK_UTEST(woven)
{

    randomized::rand_ ran;

    const size_t dims = 4;
    const size_t rmax = 3;

    matrix<unit_t>  M(rmax,dims);
    do
    {
        for(size_t i=1;i<=rmax;++i)
        {
            for(size_t j=1;j<=dims;++j)
            {
                M[i][j] = ran.in(-13,13);
            }
        }
    } while( yack::apex::flak::rank(M) < rmax );

    std::cerr << M << std::endl;

    woven::qbranch Q(dims);

    Q.initialize(M);

    std::cerr << Q << std::endl;
    
    YACK_SIZEOF(woven::qvector);
    YACK_SIZEOF(woven::qfamily);
    YACK_SIZEOF(woven::qbranch);

}
YACK_UDONE()

