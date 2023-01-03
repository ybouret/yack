
#include "yack/arith/prime-db.h"

const yack_prime_db_word yack_prime_db_data[] =
{
#include "prime-db.inc"
};

#define YACK_PRIME_DB_BPS 6

const size_t yack_prime_db_size = sizeof(yack_prime_db_data)/sizeof(yack_prime_db_data[0]);
const size_t yack_prime_db_bps = YACK_PRIME_DB_BPS;
const size_t yack_prime_db_spw = (sizeof(yack_prime_db_word)*8)/YACK_PRIME_DB_BPS;
const size_t yack_prime_db_num = yack_prime_db_spw *yack_prime_db_size;


void yack_prime_db_expand(uint8_t shift[], yack_prime_db_word u)
{
    static const uint64_t mask = (1<<yack_prime_db_bps)-1;
    size_t i=yack_prime_db_spw;
    while(i-- > 0)
    {
        shift[i] = ((uint8_t) (u&mask) + 1) << 1; u >>= yack_prime_db_bps;
    }
}
