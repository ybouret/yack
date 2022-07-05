
#include "yack/type/boolean.h"

const char yack_true[]  = "true";
const char yack_false[] = "false";

const char * yack_boolean(const int flag)
{
    return (flag?yack_true:yack_false);
}
