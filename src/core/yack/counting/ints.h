
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

    void    yack_ints_init(struct yack_ints *param, size_t n);
   
    /**! ints[1] = [n], k=1 */
    void    yack_ints_boot(struct yack_ints *param, size_t *ints);
    
    /**! next partition */
    int     yack_ints_next(struct yack_ints *param, size_t *ints);
    
#if defined(__cplusplus)
}
#endif


#endif
