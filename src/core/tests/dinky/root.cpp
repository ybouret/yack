
#include "yack/data/dinky/root.hpp"
#include "yack/data/dinky/ptr.hpp"
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/zcoop.hpp"
#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/zproxy.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    typedef dinky_node<int> node_t;
    typedef dinky_root<node_t, zproxy< znone<node_t> > > bare_root;
    typedef dinky_root<node_t, zproxy< zsolo<node_t> > > solo_root;
    typedef dinky_root<node_t, zcoop<node_t>::proxy  >   coop_root;

    typedef dinky_node< dinky_ptr<int> > pnode_t;
    typedef dinky_root<pnode_t, zproxy< znone<pnode_t> > > para_bare_root;

    
}

YACK_UTEST(dinky_root)
{
    randomized::rand_    ran;
    zcoop<node_t>::proxy fund = new zcoop<node_t>();
    
    bare_root rb;
    solo_root rs;
    coop_root rc(fund);
    
    for(size_t i=2+ran.leq(10);i>0;--i)
    {
        const int j = static_cast<int>( ran.in(-20,20) );
        rb.annex(j); rb.shove(j);
        rs.annex(j); rs.shove(j);
        rc.annex(j); rc.shove(j);
    }
    
    std::cerr << "rb=" << rb << std::endl;
    std::cerr << "rs=" << rs << std::endl;
    std::cerr << "rc=" << rc << std::endl;

    {
        bare_root rb2 = rb;
        solo_root rs2 = rs;
        coop_root rc2 = rc;
    }
    
    
    int arr[4] = { 0, 1 ,2 ,3 };
    para_bare_root prb;
    for(size_t i=0;i<4;++i)
    {
        prb.annex( &arr[i] );
    }
    std::cerr << "prb  = " << prb << std::endl;
    
    para_bare_root prb2 = prb;
    std::cerr << "prb2 = " << prb2 << std::endl;

}
YACK_UDONE()
