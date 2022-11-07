#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(16)
{
    YACK_UTEST(lib);    
    YACK_UTEST(actors);
    YACK_UTEST(components);
    YACK_UTEST(outcome);
    YACK_UTEST(eq);
    YACK_UTEST(composite);
    YACK_UTEST(reactor);
    YACK_UTEST(diff);
    YACK_UTEST(ham);
    YACK_UTEST(balance);
}
YACK_UTEST_EXEC()

