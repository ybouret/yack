
//! \file

#ifndef YACK_COUNTING_INTS_INCLUDED
#define YACK_COUNTING_INTS_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
    /**! context for integer partitions */
    struct yack_ints
    {
        size_t   n; //!< the positive integer
        size_t   k; //!< current number of  parts
    };

    void    yack_ints_init(struct       yack_perm *param, size_t n);

    
#if defined(__cplusplus)
}
#endif


#endif
