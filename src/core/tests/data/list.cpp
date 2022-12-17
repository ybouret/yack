
#include "yack/data/list/raw.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/list/cloneable.hpp"
#include "yack/data/small/node.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/offset.hpp"
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

        XNode *clone() const { return new XNode(); }

    private:
        YACK_DISABLE_ASSIGN(XNode);
    };

    template <typename LIST>
    static inline void display(const LIST &L)
    {
        std::cerr << "{";
        for(const typename LIST::node_type *node=L.head;node;node=node->next)
        {
            std::cerr << ' ' << **node;
        }
        std::cerr << " }" << std::endl;
    }
}

YACK_UTEST(data_list)
{
    randomized::rand_  ran;
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
    
    {
        std::cerr << "cxx_list/merge_back" << std::endl;
        for(size_t p=0;p<=10;++p)
        {
            for(size_t q=0;q<=10;++q)
            {
                cxx_list_of<XNode> lhs; while(lhs.size<p) lhs.push_back( new XNode() );
                YACK_ASSERT(lhs.size==p);
                cxx_list_of<XNode> rhs; while(rhs.size<q) rhs.push_back( new XNode() );
                YACK_ASSERT(rhs.size==q);

                lhs.merge_back(rhs);
                YACK_ASSERT(lhs.size==p+q);
            }
        }
        
    }
    
    {
        std::cerr << "store_increasing" << std::endl;
        DNode        nodes[1000];
        const size_t num = sizeof(nodes)/sizeof(nodes[0]);
        size_t       index[num];
        memset(nodes,0, sizeof(nodes));
        for(size_t i=0;i<num;++i)
        {
            index[i] = i;
            nodes[i].indx = i;
        }
        randomized::shuffle::data(index,num,ran);
        raw_list_of<DNode> l;
        for(size_t i=0;i<num;++i)
        {
            DNode *node = nodes+index[i];
            l.store_increasing_memory( node );
            YACK_ASSERT(l.memory_is_increasing());
        }
        l.restart();
    }
    
    
    YACK_SIZEOF(list_of<DNode>);
    YACK_SIZEOF(cxx_list_of<XNode>);
    YACK_SIZEOF(raw_list_of<DNode>);
    YACK_SIZEOF(core_list_of<DNode>);

    std::cerr << YACK_OFFSET_OF(core_list_of<DNode>,head) << std::endl;
    std::cerr << YACK_OFFSET_OF(core_list_of<DNode>,tail) << std::endl;
    std::cerr << YACK_OFFSET_OF(core_list_of<DNode>,size) << std::endl;

    std::cerr << "concrete list" << std::endl;
    klist< int,small_node<int> > L,Lbis;
    for(size_t iter=0;iter<=5;++iter)
    {
        L.append_back(  ran.full<int16_t>() );
        L.append_front( ran.full<int16_t>() );
        if(ran()>=0.2)
        {
            Lbis.add( int(iter) );
        }
    }

    std::cerr << "L=";    display(L);
    std::cerr << "Lbis="; display(Lbis);
    L.merge_back_copy(Lbis);
    std::cerr << "L=";    display(L);

    L.sort(comparison::increasing<int>);


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
    }

    {
        std::cerr << "cloneable_list/ops" << std::endl;
        cloneable_list_of<XNode> xl;
        std::cerr << "  push" << std::endl;
        for(size_t i=1+ran.leq(1000);i>0;--i)
        {
            if(ran.choice()) xl.push_back(  new XNode() );
            else             xl.push_front( new XNode() );
        }

        std::cerr << "  copy" << std::endl;
        {
            cloneable_list_of<XNode> xlc(xl);
        }
    }


    {
        std::cerr << "replacing..." << std::endl;
        cxx_list_of<XNode> xl;
        for(size_t i=2+ran.leq(1000);i>0;--i)
        {
            if(ran.choice()) xl.push_back(  new XNode() );
            else             xl.push_front( new XNode() );
        }
        std::cerr << "...within #" << xl.size << std::endl;
        for(size_t iter=1+ran.leq(10000);iter>0;--iter)
        {

            XNode *node = xl.pop( xl.get(1+ran.leq(xl.size-1)) );
            XNode *mine = xl.get(1+ran.leq(xl.size-1)) ;
            YACK_ASSERT( mine == xl.replace(mine,node) );
            xl.push_back(mine);
        }

    }

}
YACK_UDONE()

