//! \file

#ifndef YACK_COUNTING_COMB_INCLUDED
#define YACK_COUNTING_COMB_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    struct yack_comb
    {
        size_t n;
        size_t k;
        size_t nmk;
        size_t count;
    };

    int     yack_comb_init(struct yack_comb       *param, const size_t n, const size_t k);
    void    yack_comb_boot(const struct yack_comb *param, size_t *comb);
    void    yack_comb_next(const struct yack_comb *param, size_t *comb);

#if defined(__cplusplus)
}
#endif


#endif
