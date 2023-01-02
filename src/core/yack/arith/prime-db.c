
#include "yack/arith/prime-db.h"

const yack_prime_db_word yack_prime_db_data[] =
{
#include "prime-db.inc"
};

const size_t yack_prime_db_size = sizeof(yack_prime_db_data)/sizeof(yack_prime_db_data[0]);

const size_t yack_prime_db_bps = 6;
const size_t yack_prime_db_spw = (sizeof(yack_prime_db_word)*8)/yack_prime_db_bps;
