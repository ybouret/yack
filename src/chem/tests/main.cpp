#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(16)
{
    YACK_UTEST(lib);    
    YACK_UTEST(actors);
    YACK_UTEST(components);
}
YACK_UTEST_EXEC()

