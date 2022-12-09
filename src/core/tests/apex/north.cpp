#include "yack/apex/north.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"

using namespace yack;

YACK_UTEST(apex_north)
{

    YACK_SIZEOF(north::qvector<int64_t>);
    YACK_SIZEOF(north::qmatrix<int64_t>);


    YACK_SIZEOF(north::qvector<apq>);
    YACK_SIZEOF(north::qmatrix<apq>);


    for(size_t dims=1;dims<=20;++dims)
    {
        north::qmatrix<apq,memory::global> qg(dims);
        north::qmatrix<apq,memory::pooled> qp(dims);
        north::qmatrix<apq,memory::dyadic> qd(dims);

        std::cerr << "granted[" << std::setw(3) << dims << "] = "
        << std::setw(8) << qg.granted()
        << std::setw(8) << qp.granted()
        << std::setw(8) << qd.granted()
        << std::endl;
    }

}
YACK_UDONE()

