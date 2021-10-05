
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
        size_t indx;
        
    };
    
    class XNode
    {
    public:
        XNode *next;
        XNode *prev;
        inline  XNode() throw() : next(0), prev(0) {}
        inline ~XNode() throw() { assert(0==next); assert(0==prev); }
        inline  XNode(const XNode &) throw() : next(0), prev(0) {}
        
    private:
        YACK_DISABLE_ASSIGN(XNode);
    };
    
}

YACK_UTEST(data_list)
{
    uprng ran;
    {
        std::cerr << "setup/get" << std::endl;
        DNode nodes[800];
        const size_t num = sizeof(nodes)/sizeof(nodes[0]);
        memset(nodes,0, sizeof(nodes));
        
        raw_list_of<DNode> rl;
        
        for(size_t i=0;i<num;++i)
        {
            rl.push_back(nodes+i)->indx = i+1;
        }
        
        for(size_t i=num;i>0;--i)
        {
            YACK_ASSERT(i==rl.get(i)->indx);
        }
        
        rl.restart();
    }
    
    {
        std::cerr << "setup/pop" << std::endl;
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
        
    }
    
    {
        std::cerr << "cxx_list/init" << std::endl;
        cxx_list_of<XNode> xl;
        for(size_t i=1+ran.leq(1000);i>0;--i)
        {
            if(ran.choice()) xl.push_back(  new XNode() );
            else             xl.push_front( new XNode() );
        }
    }
    
    {
        std::cerr << "cxx_list/ops" << std::endl;
        cxx_list_of<XNode> xl;
        std::cerr << "  push" << std::endl;
        for(size_t i=1+ran.leq(1000);i>0;--i)
        {
            if(ran.choice()) xl.push_back(  new XNode() );
            else             xl.push_front( new XNode() );
        }
        
        std::cerr << "  copy" << std::endl;
        {
            cxx_list_of<XNode> xlc(xl);
        }
        
        std::cerr << "  mtf" << std::endl;
        for(size_t i=xl.size*2;i>0;--i)
        {
            xl.move_to_front( xl.get(1 + ran.leq(xl.size-1) ) );
        }
        
        std::cerr << "  insert_after" << std::endl;
        for(size_t i=100;i>0;--i)
        {
            XNode *mine = xl.get(1 + ran.leq(xl.size-1) );
            XNode *node = new XNode();
            xl.insert_after(mine,node);
        }
        
        std::cerr << "  insert_before" << std::endl;
        for(size_t i=100;i>0;--i)
        {
            XNode *mine = xl.get(1 + ran.leq(xl.size-1) );
            XNode *node = new XNode();
            xl.insert_before(mine,node);
        }
        
        std::cerr << "  upstream" << std::endl;
        for(size_t i=xl.size;i>1;--i)
        {
            XNode *node = xl.get(i);
            xl.towards_front(node);
        }

        std::cerr << "  pop" << std::endl;
        while(xl.size)
        {
            const size_t j = 1 + ran.leq(xl.size-1);
            delete xl.pop( xl.get(j) );
        }
        
    }
    
    
    YACK_SIZEOF(list_of<DNode>);
    YACK_SIZEOF(cxx_list_of<XNode>);
    YACK_SIZEOF(raw_list_of<DNode>);

    
}
YACK_UDONE()

