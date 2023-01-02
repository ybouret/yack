//! \file

#ifndef YACK_ARITH_PRIME_DB_INCLUDED
#define YACK_ARITH_PRIME_DB_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif


    typedef uint64_t                yack_prime_db_word;   /*!< storage word         */
    extern const yack_prime_db_word yack_prime_db_data[]; /*!< precomputed database */
    extern const size_t             yack_prime_db_size;   /*!< database size        */
    extern const size_t             yack_prime_db_bps;    /*!< bits   per shift     */
    extern const size_t             yack_prime_db_spw;    /*!< shifts per word      */
    extern const size_t             yack_prime_db_num;    /*!< fetchable shifts     */

    /** expand compressed word into shift[yack_prime_db_spw]
     */
    void yack_prime_db_expand(uint8_t shift[], yack_prime_db_word u);

#if defined(__cplusplus)
}
#endif


#endif
