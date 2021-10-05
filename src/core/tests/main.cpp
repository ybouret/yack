#include "yack/utest/driver.hpp"
#include <cstdio>

YACK_UTEST_DECL(32)
{
    YACK_UTEST(crc32);
    YACK_UTEST(error);
    YACK_UTEST(cstring);
    YACK_UTEST(excp);
    YACK_UTEST(ints);
    YACK_UTEST(base2);
    YACK_UTEST(align);
    YACK_UTEST(at_exit);

    YACK_UTEST(ram);
    YACK_UTEST(memory_chunk);
    YACK_UTEST(memory_arena);

    YACK_UTEST(data_list);
    YACK_UTEST(data_pool);

    YACK_UTEST(sync_lockable);
    YACK_UTEST(sync_primitive);
    YACK_UTEST(sync_mutex);
    YACK_UTEST(sync_condition);
    YACK_UTEST(sync_thread);
    YACK_UTEST(singleton);

}
YACK_UTEST_EXEC()
