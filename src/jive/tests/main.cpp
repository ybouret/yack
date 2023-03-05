#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(32)
{
    YACK_UTEST(tags);
    YACK_UTEST(module);
    YACK_UTEST(token);
    YACK_UTEST(firsts);
    YACK_UTEST(pattern);
    YACK_UTEST(spec);
    YACK_UTEST(viz);
    YACK_UTEST(posix);
    YACK_UTEST(regexp);
    YACK_UTEST(matching);

    YACK_UTEST(scanner);
    YACK_UTEST(analyzer);

    YACK_UTEST(syntax_types);
    YACK_UTEST(grammar);
    YACK_UTEST(parser);
    YACK_UTEST(eval);
   
    YACK_UTEST(vfs);
    YACK_UTEST(vfs_rm);

    YACK_UTEST(dynamo);

}
YACK_UTEST_EXEC()

