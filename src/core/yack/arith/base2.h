
//! \file

#ifndef YACK_BASE2_INCLUDED
#define YACK_BASE2_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    extern const size_t yack_maxi_power_of_two;                 //!< for size_t
    int                 yack_is_a_power_of_two(const size_t n); //!< check is is a power of two
    size_t              yack_next_power_of_two(size_t       v); //!< next, v<=yack_maxi_power_of_two
    size_t              yack_prev_power_of_two(const size_t v); //!< prev
#if defined(__cplusplus)
}
#endif

#endif
