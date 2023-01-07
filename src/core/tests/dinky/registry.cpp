
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
    typedef dinky_node<int> node_t;
    zcoop<node_t>::proxy fund = new zcoop<node_t>();

    dinky_registry<int,znone> rn;
    dinky_registry<int,zsolo> rs;
    dinky_registry<int,zcoop> rc(fund);
    

}
YACK_UDONE()
