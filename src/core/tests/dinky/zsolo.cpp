

#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/node.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(dinky_zsolo)
{
    typedef dinky_node<int> node_t;
    
    zsolo<node_t>   zn;
    list_of<node_t> zlist;
    while(zlist.size<=10) zlist.push_back( zn.zquery() );
    while(zlist.size) zn.zstore(zlist.pop_front());
    
    zn.reserve(10);
    std::cerr << "#solo=" << zn.stowage() << std::endl;
    
}
YACK_UDONE()

