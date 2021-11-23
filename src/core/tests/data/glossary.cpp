
#include "yack/associative/suffix/map.hpp"
#include "yack/associative/hash/map.hpp"
#include "yack/sequence/vector.hpp"
#include "../main.hpp"
#include "yack/utest/run.hpp"

using namespace yack;




YACK_UTEST(data_glossary)
{
    randomized::rand_ ran;
    {
        size_t            n = 10;
        vector<string>    keys(n,as_capacity);

        for(size_t i=n;i>0;--i)
        {
        TRIAL:
            const string tmp = bring::get<string>(ran);
            for(size_t j=keys.size();j>0;--j)
            {
                if(tmp==keys[j]) goto TRIAL;
            }
            keys.push_back(tmp);
        }

        std::cerr << "keys=" << keys << std::endl;

        suffix_map<string,size_t> s_map;
        hash_map<string,size_t>   h_map;
        for(size_t i=1;i<=n;++i)
        {
            YACK_ASSERT(s_map.insert(keys[i],i));
            YACK_ASSERT(h_map.insert(keys[i],i));
        }

        randomized::shuffle::data( keys(), keys.size(), ran);
        std::cerr << "keys=" << keys << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            YACK_ASSERT(s_map.search(keys[i]));
            YACK_ASSERT(h_map.search(keys[i]));
        }

        std::cerr << s_map << std::endl;
        std::cerr << h_map << std::endl;

    }

}
YACK_UDONE()

