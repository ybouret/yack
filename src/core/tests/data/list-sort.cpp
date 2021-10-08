
#include "yack/data/list-sort.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    class Node
    {
    public:
        Node *next, *prev;
        const size_t value;
        
        inline Node(const size_t i) throw() : next(0), prev(0), value(i)
        {
        }
        
        inline ~Node() throw()
        {
        }
        
        static inline int Compare(const Node *lhs, const Node *rhs) throw()
        {
            return comparison::increasing(lhs->value,rhs->value);
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Node);
    };
}

YACK_UTEST(data_list_sort)
{
    uprng             ran;
    cxx_list_of<Node> l;
    
    for(size_t i=0;i<100;++i)
    {
        l.push_back( new Node(i) ) ;
    }
    ran.shuffle_list(l);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
    
    merge_list_of<Node>::sort(l,Node::Compare);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
}
YACK_UDONE()

