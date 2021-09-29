#include "yack/check/crc32.h"
#include "yack/utest/run.hpp"
#include <cstring>

YACK_UTEST(crc32)
{
    const char    *data = "The quick brown fox jumps over the lazy dog";
    const uint32_t code = yack_crc32(data,strlen(data));
    std::cerr << std::hex << "crc32(" << data << ")=" << code << " / " <<  0x414FA339 << std::endl;

}
YACK_LEAVE()

