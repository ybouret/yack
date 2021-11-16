#include "yack/data/hash/table.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_hash)
{
    YACK_SIZEOF( hash_table<int>::slot_type );

    hash_table<int> itable;
    

}
YACK_UDONE()


