//! \file

#ifndef YACK_OUT_OF_REACH_INCLUDED
#define YACK_OUT_OF_REACH_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void yack_out_of_reach_zset(void *addr, const size_t size);
    void yack_out_of_reach_copy(void *target, const void *source, const size_t size);

#if defined(__cplusplus)
}
#endif

#endif
