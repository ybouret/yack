#include "yack/associative/key-mapper.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(data_key_mapper)
{
    {
        key_map<int,string> km;

        YACK_CHECK( km.insert(1,"a") );
        YACK_CHECK( km.insert(2,"b") );
        YACK_CHECK( km.insert(3,"c") );

        std::cerr << km << std::endl;

        for(key_map<int,string>::const_iterator it=km.begin();it!=km.end();++it)
        {
            std::cerr << *it << std::endl;
        }
    }

    {
        key_mapper<int,unsigned> K;
        K(1,2);
        K(3,4);
        K(6,7);
        std::cerr << K << std::endl;
        YACK_CHECK( K.forward[1] == 2 );
        YACK_CHECK( K.forward[3] == 4 );
        YACK_CHECK( K.forward[6] == 7 );
        YACK_CHECK( K.reverse[2] == 1 );
        YACK_CHECK( K.reverse[4] == 3 );
        YACK_CHECK( K.reverse[7] == 6 );

    }

    std::cerr << "sizes: " << std::endl;
    std::cerr << sizeof( key_mapper<int,int> ) << std::endl;
    std::cerr << sizeof( key_mapper<char,char> ) << std::endl;


}
YACK_UDONE()

