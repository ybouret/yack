#include "yack/system/setup.h"

const char yack_unknown[] = "unknown";
const char yack_success[] = "success";
const char yack_failure[] = "failure";

int yack_die(const char *msg)
{
    (void)msg;
    return 1;
}
