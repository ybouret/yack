
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/node.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(dinky_znone)
{
    typedef dinky_node<int> node_t;
    
    znone<node_t>   zn;
    list_of<node_t> zlist;
    while(zlist.size<=10) zlist.push_back( zn.zquery() );
    while(zlist.size) zn.zstore(zlist.pop_front());
    
}
YACK_UDONE()

