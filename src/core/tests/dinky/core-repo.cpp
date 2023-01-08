#include "yack/data/dinky/core-repo.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(dinky_core_repo)
{
    randomized::rand_ ran;
    cxx_array<string> str(3+ran.leq(10));
    
    core_repo<string>       mcr;
    core_repo<const string> ccr;

    
    for(size_t i=str.size();i>0;--i)
    {
        string &s = str[i];
        for(size_t j=1+ran.leq(10);j>0;--j) {
            s += static_cast<char>( ran.in('a','z') );
        }
        mcr << s;
        ccr << s;
    }
    
    std::cerr << str << std::endl;
    std::cerr << mcr << std::endl;
    std::cerr << ccr << std::endl;

    mcr.sort_with(string::compare);
    ccr.sort_with(comparison::decreasing<string>);
   
    std::cerr << str << std::endl;
    std::cerr << mcr << std::endl;
    std::cerr << ccr << std::endl;
    
}
YACK_UDONE()

