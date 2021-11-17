#include "yack/associative/hash/map.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "yack/kr/digest.hpp"
#include "yack/sequence/vector.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(data_hash_map)
{
    randomized::rand_ ran;

    {
        hash_map<int,apn> m;

        vector<int> keys;
        for(int i=0;i<10;++i)
        {
            keys.push_back(i);
            const apn n = bring::get<apn>(ran);
            YACK_ASSERT( !m.search(i)  );
            YACK_ASSERT( m.insert(i,n) );
        }

        for(hash_map<int,apn>::iterator it=m.begin();it!=m.end();++it)
        {
            std::cerr << *it << ", key=" << it->key() << std::endl;
        }

        {
            hash_map<int,apn> tmp(m);
            YACK_CHECK(tmp.size()==m.size());
        }

        while(keys.size())
        {
            const int &key = keys.back();
            YACK_ASSERT(  m.search(key)  );
            YACK_ASSERT(  m.remove(key)  );
            YACK_ASSERT( !m.search(key) );
            YACK_ASSERT( !m.remove(key) );
            keys.pop_back();
        }



    }

    
}
YACK_UDONE()
