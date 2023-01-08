
#include "yack/data/dinky/solo-list.hpp"
#include "yack/utest/run.hpp"

using namespace yack;



YACK_UTEST(dinky_solo_list)
{
    randomized::rand_             ran;
    dinky_solo_list<unit_t>       mcl;
    dinky_solo_list<const unit_t> ccl;
    
    for(size_t i=2+ran.leq(10);i>0;--i)
    {
        const unit_t j = ran.in(-100,100);
        mcl << j; mcl >> j;
        ccl << j; ccl >> j;
    }
    
    std::cerr << mcl << std::endl;
    std::cerr << ccl << std::endl;
    
    mcl.sort_with(comparison::increasing<unit_t>);
    ccl.sort_with(comparison::decreasing<unit_t>);
    
    std::cerr << mcl << std::endl;
    std::cerr << ccl << std::endl;
    
    mcl.clear();
    ccl.clear();
    std::cerr << "caches: "
    << mcl.ready() << " / "
    << ccl.ready()
    << std::endl;
}
YACK_UDONE()

