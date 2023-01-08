

#include "yack/data/dinky/coop-repo.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(dinky_coop_repo)
{
    randomized::rand_ ran;
    cxx_array<string> str(3+ran.leq(10));
    
    typedef dinky_coop_repo<const string> csr_type;
    csr_type::proxy_type cache = new csr_type::zpool_type();
    csr_type ccr(cache);
    
    for(size_t i=str.size();i>0;--i)
    {
        string &s = str[i];
        for(size_t j=1+ran.leq(10);j>0;--j) {
            s += static_cast<char>( ran.in('a','z') );
        }
        ccr << s;
    }
    
    std::cerr << str << std::endl;
    std::cerr << ccr << std::endl;
    
    ccr.sort_with(comparison::decreasing<string>);
    
    std::cerr << str << std::endl;
    std::cerr << ccr << std::endl;
    
    ccr.clear();
    std::cerr << "caches: " << cache->stowage() << std::endl;
    
}
YACK_UDONE()

