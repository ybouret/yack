
//! \file

#ifndef YACK_BASE2_INCLUDED
#define YACK_BASE2_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    extern const size_t yack_maxi_power_of_two;
    int                 yack_is_a_power_of_two(const size_t n);
    size_t              yack_next_power_of_two(size_t       v);

#if defined(__cplusplus)
}
#endif

#endif
