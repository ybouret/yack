

#include "yack/data/pool-sort.hpp"
#include "yack/data/cxx-pool.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    class Node
    {
    public:
        Node *next;
        const size_t value;
        
        inline Node(const size_t i) throw() : next(0),  value(i)
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

YACK_UTEST(data_pool_sort)
{
    randomized::rand_  ran;
    cxx_pool_of<Node> l;
    
    for(size_t i=0;i<100;++i)
    {
        l.store( new Node(i) ) ;
    }
    //ran.shuffle_pool(l);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
    
    merge_pool_of<Node>::sort(l,Node::Compare);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
    
    merge_pool_of<Node>::sort_by_increasing_address(l);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
    YACK_CHECK(l.memory_is_increasing());

    merge_pool_of<Node>::sort_by_decreasing_address(l);
    for(const Node *node=l.head;node;node=node->next)
    {
        std::cerr << node->value << '/';
    }
    std::cerr << std::endl;
    YACK_CHECK(l.memory_is_decreasing());

    
}
YACK_UDONE()

