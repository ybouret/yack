#include "yack/system/endian.hpp"

namespace yack {

    bool endianness:: BE() throw()
    {
        static const union {
            uint32_t i;
            uint8_t  c[4];
        } alias = {0x01020304};
        static const bool result = (alias.c[0] == 1);
        return result;
    }

    bool endianness:: LE() throw()
    {
        return !BE();
    }
}
