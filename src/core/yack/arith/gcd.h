
//! \file

#ifndef YACK_ARITH_GCD_INCLUDED
#define YACK_ARITH_GCD_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! GCD of any 64bits*/
    uint64_t yack_gcd64(uint64_t a, uint64_t b);

    /**! GCD of postive a and b **/
    uint64_t yack_gcd64_(uint64_t a, uint64_t b);


    /**! LCM of any 64bits*/
    uint64_t yack_lcm64(uint64_t a, uint64_t b);


    /**! simplify (*a)/(*b>0) */
    void     yack_simplify64(uint64_t *a, uint64_t *b);

#if defined(__cplusplus)
}
#endif


#endif
