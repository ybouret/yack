

#include "yack/associative/suffix/map.hpp"
#include "yack/associative/suffix/key-walker.hpp"

#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(data_suffix_map)
{
    randomized::rand_ ran;

    suffix_map<int,apn> m;
    vector<int>         keys;
    for(int i=0;i<12;++i)
    {
        const apn n = bring::get<apn>(ran);
        keys.push_back(i);
        YACK_ASSERT( !m.search(i)   );
        YACK_ASSERT(  m.insert(i,n) );
        YACK_ASSERT(  m.search(i)   );
    }
    std::cerr << "keys=" << keys << std::endl;
    randomized::shuffle::data(keys(),keys.size(),ran);
    std::cerr << "keys=" << keys << std::endl;
    YACK_CHECK( keys.size() == m.size() );
    while( keys.size() )
    {
        const int &key = keys.back();
        YACK_ASSERT(  m.search(key)   );
        YACK_ASSERT(  m.remove(key)   );
        YACK_ASSERT(  !m.search(key)   );
        YACK_ASSERT(  !m.remove(key)   );
        keys.pop_back();
        YACK_ASSERT(keys.size() == m.size() );

    }


}
YACK_UDONE()

