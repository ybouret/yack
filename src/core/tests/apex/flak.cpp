#include "yack/apex/flak.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace apex;

template <typename T>
static inline void test_colinear()
{
#define CHECK_COLINEAR(FLAGS) \
std::cerr << alpha << " / " << beta << std::endl; \
YACK_CHECK(   flak::colinear(alpha,beta) == FLAGS )

    {
        vector<int> alpha;
        vector<int> beta;
        alpha << 1;
        beta  << 2;
        CHECK_COLINEAR(true);
        alpha << 0;
        beta  << 0;
        CHECK_COLINEAR(true);
        alpha << -1;
        beta  << -1;
        CHECK_COLINEAR(false);
        beta.back() = -2;
        CHECK_COLINEAR(true);
    }
}

YACK_UTEST(apex_flak)
{
    vector<apq> Q;
    Q << apq(1,2) << apq(1,3);
    std::cerr << Q << " => " << flak::common_denominator(Q) << std::endl;

    test_colinear<int>();
    test_colinear<short>();
    test_colinear<apq>();


}
YACK_UDONE()

