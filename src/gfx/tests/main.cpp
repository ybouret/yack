#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(16)
{
    YACK_UTEST(types);
    YACK_UTEST(png_io);
    YACK_UTEST(png);
    YACK_UTEST(tess);
}
YACK_UTEST_EXEC()

