
//! \file

#ifndef YACK_COUNTING_PERM_INCLUDED
#define YACK_COUNTING_PERM_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    struct yack_perm
    {
        size_t   n;
    };

    void    yack_perm_boot(const struct yack_perm *param, size_t *perm);
    void    yack_perm_next(const struct yack_perm *param, size_t *perm);

#if defined(__cplusplus)
}
#endif


#endif
