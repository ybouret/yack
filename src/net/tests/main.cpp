
#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(16)
{
    YACK_UTEST(types);
    YACK_UTEST(init);
    YACK_UTEST(socket);
    YACK_UTEST(addr);
    YACK_UTEST(resolve);

    YACK_UTEST(tcp_client);
    
}
YACK_UTEST_EXEC()
