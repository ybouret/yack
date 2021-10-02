#include "yack/utest/driver.hpp"
#include <cstdio>

YACK_UTEST_DECL(16)
{
    YACK_UTEST(crc32);
    YACK_UTEST(error);
    YACK_UTEST(cstring);
    YACK_UTEST(excp);
    YACK_UTEST(ints);
    YACK_UTEST(base2);
    YACK_UTEST(align);

    YACK_UTEST(ram);
    YACK_UTEST(memory_chunk);
    YACK_UTEST(memory_arena);

    YACK_UTEST(data_list);
}
YACK_UTEST_EXEC()
