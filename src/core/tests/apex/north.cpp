#include "yack/apex/north/qmatrix.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/container/matrix.hpp"

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

    {
        north:: qmatrix<apq,memory::global> U(3);

        matrix<int> vec(3,3);
        vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
        vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
        vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;

        YACK_CHECK(U.grow(vec[1])); std::cerr << U << std::endl;
        YACK_CHECK(U.grow(vec[2])); std::cerr << U << std::endl;
        YACK_CHECK(U.grow(vec[3])); std::cerr << U << std::endl;



    }


}
YACK_UDONE()

