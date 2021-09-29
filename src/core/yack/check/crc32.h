//! \file

#ifndef YACK_CRC32_INCLUDED
#define YACK_CRC32_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    uint32_t yack_crc32_run(uint32_t    crc,
                            const void *addr,
                            size_t      size);

    uint32_t yack_crc32(const void *addr, size_t size);

#if defined(__cplusplus)
}
#endif

#endif
