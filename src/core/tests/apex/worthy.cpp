#include "yack/apex/worthy.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/data/small/set.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(apex_worthy)
{
    worthy::qfamily U(3);
    worthy::qfamily V(3);


    matrix<int> vec(3,3);
    vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
    vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
    vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;

    YACK_CHECK(U.grow(vec[1]));
    std::cerr << U.project(vec[1]) << std::endl;
    YACK_CHECK(U.grow(vec[2]));
    std::cerr << U.project(vec[1]) << std::endl;
    std::cerr << U.project(vec[2]) << std::endl;
    YACK_CHECK(U.grow(vec[3]));
    std::cerr << U.project(vec[1]) << std::endl;
    std::cerr << U.project(vec[2]) << std::endl;
    std::cerr << U.project(vec[3]) << std::endl;

    std::cerr << U << std::endl;

    YACK_CHECK(V.grow(vec[3]));
    YACK_CHECK(V.grow(vec[2]));
    YACK_CHECK(V.grow(vec[1]));

    std::cerr << V << std::endl;

    V.reset();
    YACK_CHECK(V.grow(vec[1])); YACK_CHECK(!(V==U));
    YACK_CHECK(V.grow(vec[2])); YACK_CHECK(!(V==U));
    YACK_CHECK(V.grow(vec[3])); YACK_CHECK( (V==U));




    randomized::rand_ ran;
    for(size_t iter=0;iter<8;++iter)
    {
        bring::fill(vec[1],ran);
        YACK_CHECK(!U.grow(vec[1]));
    }
    


    
    
}
YACK_UDONE()

