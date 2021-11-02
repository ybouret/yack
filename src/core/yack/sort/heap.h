
//! \file

#ifndef YACK_SORT_HEAP_C_INCLUDED
#define YACK_SORT_HEAP_C_INCLUDED 1


#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void hpsort(const size_t  n,
                float         ra[],
                int (*proc)(const void *lhs, const void *rhs, void *args),
                void *args);


#if defined(__cplusplus)
}
#endif


#endif
