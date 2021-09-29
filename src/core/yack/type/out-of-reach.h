//! \file

#ifndef YACK_OUT_OF_REACH_INCLUDED
#define YACK_OUT_OF_REACH_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void       *yack_out_of_reach_addr(void *addr);        //!< same address
    const void *yack_out_of_reach_const(const void *addr); //!< same address
    void        yack_out_of_reach_zset(void *addr, const size_t size); //!< memset(addr,0,size)
    void        yack_out_of_reach_copy(void *target, const void *source, const size_t size); //!< memcpy(target,source,size)
    void        yack_out_of_reach_move(void *target, const void *source, const size_t size); //!< memmove(target,source,size)

#if defined(__cplusplus)
}
#endif

#endif
