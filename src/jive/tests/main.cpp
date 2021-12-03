#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(8)
{
    YACK_UTEST(tags);
    YACK_UTEST(module);
    YACK_UTEST(token);
    YACK_UTEST(firsts);
    YACK_UTEST(pattern);
    YACK_UTEST(spec);
    YACK_UTEST(viz);
}
YACK_UTEST_EXEC()

