#include "yack/memory/sentry.hpp"
#include "yack/utest/run.hpp"
#include "yack/hashing/sha1.hpp"

using namespace yack;

YACK_UTEST(memory_sentry)
{
    YACK_SIZEOF(hashing::sha1);
    YACK_SIZEOF(memory::sentry);

    {
        int result = 45;
        YACK_MEM_SENTRY(NULL,0);
        YACK_MEM_SENTRY(NULL,0);
        YACK_MEM_SENTRY_ON(result);
    }
}
YACK_UDONE()

