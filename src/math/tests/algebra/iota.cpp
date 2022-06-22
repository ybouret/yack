#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"
#include "core/tests/main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/type/v2d.hpp"
#include "yack/type/v3d.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(iota)
{

    v2d<int> iv2(1,2);
    v3d<int> iv3(3,4,5);
    std::cerr << "iv2=" << iv2 << "; iv3=" << iv3 << std::endl;

    iota::load(iv2,iv3);
    std::cerr << "iv2=" << iv2 << std::endl;
    YACK_CHECK(iv2.x==iv3.x);
    YACK_CHECK(iv2.y==iv3.y);

    iv3.x = 1;
    iv3.y = 2;
    iota::save(iv3,iv2);
    std::cerr << "iv2=" << iv2 << std::endl;
    YACK_CHECK(iv2.x==iv3.x);
    YACK_CHECK(iv2.y==iv3.y);

}
YACK_UDONE()


