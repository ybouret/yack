
#include "yack/data/raw-list.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    struct DNode
    {
        DNode *next;
        DNode *prev;
    };
}

YACK_UTEST(data_list)
{
    uprng ran;
    DNode nodes[256];
    memset(nodes,0, sizeof(nodes));
    
    raw_list_of<DNode> rl;
    for(size_t i=0;i<sizeof(nodes)/sizeof(nodes[0]);++i)
    {
        if(ran.choice()) rl.push_back(nodes+i);
        else             rl.push_front(nodes+i);
    }
    
    rl.reset();
    
}
YACK_UDONE()

