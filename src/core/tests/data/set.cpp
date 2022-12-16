#include "yack/data/set.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(data_set)
{

    ds_zpool<int>::pointer izp = new ds_zpool<int>();
    izp->reserve(100);

    data_set<int> ids( izp );

    YACK_CHECK(!ids.contains(4));

    YACK_CHECK(ids.insert(12));
    YACK_CHECK(ids.insert(13));
    std::cerr << *ids << std::endl;
    ids.free();
    YACK_CHECK(ids.insert(13));
    YACK_CHECK(ids.insert(12));
    YACK_CHECK(ids.insert(4));
    YACK_CHECK(ids.insert(15));

    std::cerr << *ids << std::endl;
    YACK_CHECK(ids.contains(4));
    YACK_CHECK(ids.contains(15));




    YACK_SIZEOF(ds_zpool<int>);
    YACK_SIZEOF(ds_zpool<string>);
    YACK_SIZEOF(data_set<int>);
    YACK_SIZEOF(data_set<string>);

}
YACK_UDONE()

