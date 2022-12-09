#include "yack/apex/north.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"

using namespace yack;

YACK_UTEST(apex_north)
{

    YACK_SIZEOF(north::qvector<int64_t>);
    YACK_SIZEOF(north::qmatrix<int64_t>);


    YACK_SIZEOF(north::qvector<apq>);
    YACK_SIZEOF(north::qmatrix<apq>);


    for(size_t dims=1;dims<=8;++dims)
    {
        north::qmatrix<apq,memory::global> qg(dims);
        north::qmatrix<apq,memory::dyadic> qd(dims);

        std::cerr << "granted[" << std::setw(3) << dims << "] = "
        << std::setw(6) << qg.granted()
        << std::setw(6) << qd.granted()
        << std::endl;
    }

}
YACK_UDONE()

