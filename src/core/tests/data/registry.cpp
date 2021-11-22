#include "yack/utest/run.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/associative/hash/set.hpp"
#include "yack/sequence/vector.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(data_registry)
{
    randomized::rand_ ran;
    size_t            n = 10;
    vector<string>    keys(n,as_capacity);
    
    for(size_t i=n;i>0;--i)
    {
        const string tmp = bring::get<string>(ran);
        keys.push_back(tmp);
    }
    
    std::cerr << "keys=" << keys << std::endl;
    
    suffix_set<string,size_t> s_set;
    hash_set<string,size_t>   h_set;
    for(size_t i=0;i<n;++i)
    {
        
    }
    
    
}
YACK_UDONE()

