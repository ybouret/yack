#include "yack/utest/driver.hpp"

using namespace yack;


YACK_UTEST_DECL(16)
{
	YACK_UTEST(matte);
    YACK_UTEST(canvas);
    YACK_UTEST(jpeg);
}
YACK_UTEST_EXEC()

