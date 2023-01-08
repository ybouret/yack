
#include "yack/data/dinky/registry.hpp"
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/zcoop.hpp"
#include "yack/data/dinky/zproxy.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    
    
}

YACK_UTEST(dinky_registry)
{
    randomized::rand_ ran;
    typedef dinky_node<int> node_t;
    zcoop<node_t>::proxy fund = new zcoop<node_t>();

    dinky_registry<int,znone> rn;
    dinky_registry<int,zsolo> rs;
    dinky_registry<int,zcoop> rc(fund);

    for(size_t n=2+ran.leq(10);n>0;--n)
    {
        const int i = static_cast<int>(ran.in(-10,10));
        rn << i; rn >> i;
        rs << i; rs >> i;
        rc << i; rc >> i;
    }
    
    std::cerr << rn << std::endl;
    std::cerr << rs << std::endl;
    std::cerr << rc << std::endl;


}
YACK_UDONE()
