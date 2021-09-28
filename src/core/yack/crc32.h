
#ifndef YACK_CRC32_INCLUDED
#define YACK_CRC32_INCLUDED 1

#include "yack/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    uint32_t YACK_CRC32(uint32_t    crc,
                        const void *addr,
                        size_t      size);

#if defined(__cplusplus)
}
#endif

#endif
