
//! \file

#ifndef YACK_INFO_MODULATION_H_INCLUDED
#define YACK_INFO_MODULATION_H_INCLUDED 1


#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! modulation function */
    typedef uint8_t (*yack_modulation)(const uint8_t x, void *args);

#if defined(__cplusplus)
}
#endif

#endif
