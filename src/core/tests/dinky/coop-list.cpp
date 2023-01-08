

#include "yack/data/dinky/coop-list.hpp"
#include "yack/utest/run.hpp"

using namespace yack;



YACK_UTEST(dinky_coop_list)
{
    randomized::rand_         ran;
    typedef coop_list<unit_t> mlist;
    mlist::proxy_type mproxy = new mlist::zpool_type();
    mlist             mcl(mproxy);
    
    for(size_t i=2+ran.leq(10);i>0;--i)
    {
        const unit_t j = ran.in(-100,100);
        mcl << j; mcl >> j;
     }
    
    std::cerr << mcl << std::endl;
 
    mcl.sort_with(comparison::increasing<unit_t>);
 
    std::cerr << mcl << std::endl;
 
    mcl.clear();
    std::cerr << "caches: " << mproxy->stowage() << std::endl;
}
YACK_UDONE()

