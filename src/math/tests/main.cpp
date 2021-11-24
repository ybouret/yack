#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(80)
{
    YACK_UTEST(numeric);
    YACK_UTEST(triplet);

    YACK_UTEST(lu);

    YACK_UTEST(rk4);

    YACK_UTEST(root_zalg);
}
YACK_UTEST_EXEC()
