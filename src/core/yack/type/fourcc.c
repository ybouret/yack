
#include "yack/type/fourcc.h"

const char *yack_fourcc(const uint32_t code)
{
    static char buf[8] = { 0 };
    buf[0] = (char)(YACK_FOURCC_AT(0,code));
    buf[1] = (char)(YACK_FOURCC_AT(1,code));
    buf[2] = (char)(YACK_FOURCC_AT(2,code));
    buf[3] = (char)(YACK_FOURCC_AT(3,code));
    return buf;
}
