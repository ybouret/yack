//! \file

#ifndef YACK_COUNTING_COMB_INCLUDED
#define YACK_COUNTING_COMB_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! context for combinations */
    struct yack_comb
    {
        size_t n;        //!< width
        size_t k;        //!< space
        size_t nmk;      //!< n-k
    };

    /**! setup */
    void    yack_comb_init(struct yack_comb       *param, const size_t n, const size_t k);

    /**! comb[1..k]=[1..k]*/
    void    yack_comb_boot(const struct yack_comb *param, size_t *comb);

    /**! next combination */
    void    yack_comb_next(const struct yack_comb *param, size_t *comb);

#if defined(__cplusplus)
}
#endif


#endif
