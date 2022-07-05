/** \file */

#ifndef YACK_TYPE_BOOLEAN_INCLUDED
#define YACK_TYPE_BOOLEAN_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    extern const char yack_true[];  /**< "true"  */
    extern const char yack_false[]; /**< "false" */

    /** return true/false */
    const char * yack_boolean(const int flag);

#if defined(__cplusplus)
}
#endif

#endif

