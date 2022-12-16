#include "yack/data/set.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(data_set)
{

    ds_zpool<int>::pointer izp = new ds_zpool<int>();
    izp->reserve(100);

    data_set<int> ids( izp );

    std::cerr << "Inserting On Empty" << std::endl;
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(!ids.insert(10));
    YACK_ASSERT(!ids.contains(1));
    YACK_ASSERT(!ids.contains(100));
    std::cerr << std::endl;

    std::cerr << "Inserting On Size=1, Lower" << std::endl;
    ids.free();
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.insert(1));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(ids.contains(1));
    YACK_ASSERT(!ids.contains(0));
    YACK_ASSERT(!ids.contains(11));
    YACK_ASSERT(!ids.contains(5));

    std::cerr << "Inserting On Size=1, Upper" << std::endl;
    ids.free();
    YACK_ASSERT(ids.insert(1));
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(ids.contains(1));
    YACK_ASSERT(!ids.contains(0));
    YACK_ASSERT(!ids.contains(11));
    YACK_ASSERT(!ids.contains(5));




    
}
YACK_UDONE()

