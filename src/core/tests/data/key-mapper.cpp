#include "yack/associative/key-mapper.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"

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

    {
        key_mapper<size_t,size_t> imap;
        imap(2,3);
        imap(4,1);
        imap(6,2);
        std::cerr << imap << std::endl;

        vector<double> gvec(6,0);
        vector<double> lvec(3,0);
        for(size_t i=6;i>0;--i) gvec[i] = i;

        imap.send(lvec,gvec);
        std::cerr << "gvec=" << gvec << " => " << lvec << std::endl;
        for(int i=3;i>0;--i)  lvec[i] = -i;
        imap.recv(gvec,lvec);
        std::cerr << "lvec=" << lvec << " => " << gvec << std::endl;

    }


}
YACK_UDONE()

