


#include "yack/data/dinky/zproxy.hpp"
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/zcoop.hpp"

#include "yack/data/dinky/node.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    typedef dinky_node<int> node_t;
    
    template <typename ZPOOL>
    static inline void zcheck(ZPOOL &pool)
    {
        pool->reserve(10);
    }
}

YACK_UTEST(dinky_zproxy)
{
    
    YACK_SIZEOF(znone<node_t>);
    YACK_SIZEOF(zsolo<node_t>);
    YACK_SIZEOF(zcoop<node_t>);

    zproxy< znone<node_t> > znone_proxy;
    zproxy< zsolo<node_t> > zsolo_proxy;
    zcoop<node_t>::pointer  zcoop_proxy = new zcoop<node_t>();
    zcheck(znone_proxy);
    zcheck(zsolo_proxy);
    zcheck(zcoop_proxy);
}
YACK_UDONE()

