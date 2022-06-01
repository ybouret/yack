#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(16)
{
    YACK_UTEST(library);
    YACK_UTEST(components);
    YACK_UTEST(solve);
    YACK_UTEST(eq);
    YACK_UTEST(eqs);
    YACK_UTEST(plexus);
    YACK_UTEST(dist);
    YACK_UTEST(comb);
    YACK_UTEST(reactor);
}
YACK_UTEST_EXEC()

