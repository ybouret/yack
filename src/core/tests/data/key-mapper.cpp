#include "yack/associative/key-mapper.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_key_mapper)
{
    {
        key_map<int> km;

        YACK_CHECK( km.insert(1,3) );
        YACK_CHECK( km.insert(2,5) );
        YACK_CHECK( km.insert(3,7) );

        std::cerr << km << std::endl;

        for(key_map<int>::const_iterator it=km.begin();it!=km.end();++it)
        {
            std::cerr << *it << std::endl;
        }
    }

    {
        key_mapper<int> K;
        K(1,2);
        K(3,2);

    }
}
YACK_UDONE()

