#include "yack/system/setup.h"

const char yack_unknown[] = "unknown";
const char yack_success[] = "success";
const char yack_failure[] = "failure";

int yack_die(const char *msg)
{
    (void)msg;
    return 1;
}

int yack_good(const void *addr, const size_t size)
{
    if(NULL!=addr)
    {
        return 1;
    }
    else
    {
        assert(NULL==addr);
        return size<=0 ? 1 : 0;
    }

}
