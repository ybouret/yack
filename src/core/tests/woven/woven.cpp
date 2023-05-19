#include "yack/woven/qvector.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"

using namespace yack;

YACK_UTEST(woven)
{
    
    vector<int>    v1; v1 << -1 << 2;
    woven::qvector q1(v1);

    std::cerr << "v1=" << v1 << std::endl;
    std::cerr << "q1=" << q1 << "@" << q1.norm2 << std::endl;

    YACK_SIZEOF(woven::qvector);

    cxx_array<apq> arr(q1.dimensions);
    cxx_array<int> vec(q1.dimensions);

    vec[1] = 7;
    vec[2] = 13;
    std::cerr << "vec=" << vec << std::endl;

    YACK_CHECK(q1.ortho(arr,vec));
    std::cerr << "arr=" << arr << std::endl;
    woven::qvector q2(arr);
    std::cerr << "q2=" << q2 << "@" << q2.norm2 << std::endl;


}
YACK_UDONE()

