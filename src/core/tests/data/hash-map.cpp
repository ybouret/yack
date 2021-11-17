#include "yack/associative/hash/map.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "yack/kr/digest.hpp"

using namespace yack;

YACK_UTEST(data_hash_map)
{
    {
        hash_map<int,apn> m;
        m.search(5);

    }

    {
        hash_map<digest,size_t> m;
    }

}
YACK_UDONE()
