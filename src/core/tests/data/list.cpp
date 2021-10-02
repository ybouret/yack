
#include "yack/data/raw-list.hpp"
#include "yack/data/cxx-list.hpp"
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
    
    class XNode
    {
    public:
        XNode *next;
        XNode *prev;
        inline  XNode() throw() : next(0), prev(0) {}
        inline ~XNode() throw() { assert(0==next); assert(0==prev); }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(XNode);
    };
    
}

YACK_UTEST(data_list)
{
    uprng ran;
    {
        DNode nodes[256];
        memset(nodes,0, sizeof(nodes));
        
        raw_list_of<DNode> rl;
        
        for(size_t i=0;i<sizeof(nodes)/sizeof(nodes[0]);++i)
        {
            if(ran.choice()) rl.push_back(nodes+i);
            else             rl.push_front(nodes+i);
        }
        
        while( rl.size )
        {
            if(ran.choice()) rl.pop_back();
            else             rl.pop_front();
        }
        
        rl.restart();
    }
    
    {
        cxx_list_of<XNode> xl;
        for(size_t i=1+ran.leq(1000);i>0;--i)
        {
            if(ran.choice()) xl.push_back(  new XNode() );
            else             xl.push_front( new XNode() );
        }
    }
    
}
YACK_UDONE()

