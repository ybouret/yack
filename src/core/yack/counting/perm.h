
//! \file

#ifndef YACK_COUNTING_PERM_INCLUDED
#define YACK_COUNTING_PERM_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! context for permutatons */
    struct yack_perm
    {
        size_t   n; //!< among n, count will be n!
    };

    /**! set n */
    void    yack_perm_init(struct       yack_perm *param, size_t n);

    /**! perm[1..n] = [1..n] */
    void    yack_perm_boot(const struct yack_perm *param, size_t *perm);

    /**! next permutation */
    int    yack_perm_next(const struct yack_perm *param, size_t *perm);

#if defined(__cplusplus)
}
#endif


#endif
