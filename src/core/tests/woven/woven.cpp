#include "yack/woven/qbuilder.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/container/matrix.hpp"
#include "../main.hpp"
#include "yack/apex/flak.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/utest/tmx.hpp"
#include "yack/woven/qcompress.hpp"

using namespace yack;



YACK_UTEST(woven)
{

    randomized::rand_ ran;
    wtime chrono;


    size_t dims = 4; if(argc>1) dims = ios::ascii::convert::to<size_t>(argv[1], "dims");
    size_t ndof = 3; if(argc>2) ndof = ios::ascii::convert::to<size_t>(argv[2], "ndof");

    matrix<unit_t> M0(ndof,dims);
    for(size_t i=1;i<=ndof;++i)
    {
        for(size_t j=1;j<=dims;++j)
        {
            M0[i][j] = ran.in(-3,3);
        }
    }
    matrix<int> M;
    const size_t rank = woven::qcompress::build(M,M0);

    std::cerr << "M0=" << M0 << std::endl;
    std::cerr << "M =" << M  << std::endl;
    std::cerr << "rank=" << rank << std::endl;

    woven::zrepo    repo1(dims);
    woven::zrepo    repo2(dims);

    //std::cerr << "With Reduction" << std::endl;
    //woven::qbranch::doReduce = true;
    uint64_t n64 = 0;
    {
        woven::qbuilder Q(dims);
        n64 = Q(repo1,M,rank,false);
        (std::cerr << "--> done" << std::endl).flush();
    }


    std::cerr << "Without Reduction" << std::endl;
    woven::qbranch::doReduce = false;
    uint64_t r64 = 0;
    {
        woven::qbuilder Q(dims);
        r64 = Q(repo2,M,rank,false);
        (std::cerr << "--> done" << std::endl).flush();
    }

    std::cerr << "M=" << M << std::endl;
    repo1.sort();
    std::cerr << repo1 << std::endl;
    repo2.sort();
    std::cerr << repo2 << std::endl;

    std::cerr << "red: " << chrono(n64) << " / raw: " << chrono(r64) << std::endl;

    YACK_SIZEOF(woven::zvector);
    YACK_SIZEOF(woven::qvector);
    YACK_SIZEOF(woven::qfamily);
    YACK_SIZEOF(woven::qbranch);

}
YACK_UDONE()

