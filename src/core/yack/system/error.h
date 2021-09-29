//! \file

#ifndef YACK_OUT_OF_REACH_INCLUDED
#define YACK_OUT_OF_REACH_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void yack_bsd_format_error(char *buffer, const size_t length, const int      err);

#if defined(Y_WIN)
    void yack_win_format_error(char *buffer, const size_t length, const uint32_t err);
#endif


#if defined(__cplusplus)
}
#endif

#endif

