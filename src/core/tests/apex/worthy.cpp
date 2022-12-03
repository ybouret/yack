#include "yack/apex/worthy.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(apex_worthy)
{
    worthy::qfamily U(3);
    
    U.free();
    vector<int> vec(3);
    vec[1] = 1; vec[2] = 1; vec[3] = 1;
    YACK_CHECK(U.grow(vec));
    std::cerr << *U << std::endl;

    
    vec.ld(0);
    vec[1] = 1;
    YACK_CHECK(U.grow(vec));
    std::cerr << *U << std::endl;

    
    
}
YACK_UDONE()

