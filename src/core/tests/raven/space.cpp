#include "yack/raven/qvector.hpp"
#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(raven_space)
{

    const size_t  dim = 2;
    vector<apz>   data(dim);
    data[1] = 1;


    raven::qvector u1( &data[1], dim );
    u1.normalize();
    std::cerr << "dimension=" << u1.dimension << " / " << u1.size() << std::endl;
    std::cerr << "u1=" << u1 << std::endl;

    vector<apz> v(dim);

    v[1] = 1;
    v[2] = 1;

    std::cerr << "v=" << v << std::endl;
    vector<apq> u(v,transmogrify);
    std::cerr << "u=" << u << std::endl;

    u1.sub(u,v);
    std::cerr << "u=" << u << std::endl;



}
YACK_UDONE()

