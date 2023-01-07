

#include "yack/data/dinky/manifest.hpp"
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/zcoop.hpp"
#include "yack/data/dinky/zproxy.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    
    
}

YACK_UTEST(dinky_manifest)
{
    typedef dinky_node< dinky_ptr<int> > node_t;
    zcoop<node_t>::proxy fund = new zcoop<node_t>();
    
    dinky_manifest<int,znone> rn;
    dinky_manifest<int,zsolo> rs;
    dinky_manifest<int,zcoop> rc(fund);
}
YACK_UDONE()
