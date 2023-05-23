#include "yack/woven/qbuilder.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/container/matrix.hpp"
#include "../main.hpp"
#include "yack/apex/flak.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/utest/tmx.hpp"

using namespace yack;

namespace yack
{
    namespace  woven
    {
        bool qbranch::doReduce = true;
    }
}
YACK_UTEST(woven)
{

    randomized::rand_ ran;
    wtime chrono;


    size_t dims = 4; if(argc>1) dims = ios::ascii::convert::to<size_t>(argv[1], "dims");
    size_t ndof = 3; if(argc>2) ndof = ios::ascii::convert::to<size_t>(argv[2], "ndof");

    matrix<unit_t> M(ndof,dims);
    for(size_t i=1;i<=ndof;++i)
    {
        for(size_t j=1;j<=dims;++j)
        {
            M[i][j] = ran.in(-3,3);
        }
    }
    const size_t rank = apex::flak::rank(M);

    std::cerr << "M=" << M << std::endl;
    std::cerr << "rank=" << rank << std::endl;

    woven::indices::fund fund = new woven::indices::bank();
    woven::qbranch Q(dims,fund);
    woven::qbranch Qng(dims,fund);
    woven::zrepo   repo(dims);

    std::cerr << "With Reduction" << std::endl;
    woven::qbranch::doReduce = false;
    uint64_t  n64 = 0;
    YACK_TMX_ADD(n64,Q.initialize(M,repo,false));
    while(Q.size)
    {
        YACK_TMX_ADD(n64,
                     Qng.generate(Q,M,rank,repo);
                     Q.swap_with(Qng)
                     );
        (std::cerr << '.').flush();
    }
    std::cerr << std::endl;

    std::cerr << "Without Reduction" << std::endl;
    woven::qbranch::doReduce = false;
    woven::zrepo   repo2(dims);
    uint64_t  r64 = 0;
    YACK_TMX_ADD(r64,Q.initialize(M,repo,false));
    while(Q.size)
    {
        YACK_TMX_ADD(r64,
                     Qng.generate(Q,M,rank,repo);
                     Q.swap_with(Qng)
                     );
        (std::cerr << '.').flush();
    }
    std::cerr << std::endl;

    std::cerr << "M=" << M << std::endl;
    repo.sort();
    std::cerr << repo << std::endl;
    repo2.sort();
    std::cerr << repo2 << std::endl;

    std::cerr << "red: " << chrono(n64) << " / raw: " << chrono(r64) << std::endl;

    YACK_SIZEOF(woven::zvector);
    YACK_SIZEOF(woven::qvector);
    YACK_SIZEOF(woven::qfamily);
    YACK_SIZEOF(woven::qbranch);

}
YACK_UDONE()

