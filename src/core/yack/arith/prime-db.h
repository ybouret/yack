

//! \file

#ifndef YACK_ARITH_GCD_INCLUDED
#define YACK_ARITH_GCD_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif


    typedef uint64_t                yack_prime_db_word;
    extern const yack_prime_db_word yack_prime_db_data[];
    extern const size_t             yack_prime_db_size;
    extern const size_t             yack_prime_db_bps; /**! bits   per shift  */
    extern const size_t             yack_prime_db_spw; /**! shifts per word   */
    extern const size_t             yack_prime_db_num; /**! fetchable shifts  */
    

#if defined(__cplusplus)
}
#endif


#endif
