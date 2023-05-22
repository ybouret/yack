#include "yack/woven/qbranch.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/container/matrix.hpp"
#include "../main.hpp"
#include "yack/apex/flak.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

YACK_UTEST(woven)
{

    randomized::rand_ ran;


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

    Q.initialize(M,repo,false);
    std::cerr << Q << std::endl;
    //std::cerr << "repo: " << repo << std::endl;
    while(Q.size)
    {
        Qng.generate(Q,M,rank,repo);
        Q.swap_with(Qng);
        std::cerr << Q << std::endl;
    }

    std::cerr << "M=" << M << std::endl;
    size_t i=1;
    for(const woven::zvector *Z=repo.head;Z;Z=Z->next,++i)
    {
        std::cerr << "\tV" << i << " = " << *Z << std::endl;
    }
    YACK_SIZEOF(woven::qvector);
    YACK_SIZEOF(woven::qfamily);
    YACK_SIZEOF(woven::qbranch);

}
YACK_UDONE()

