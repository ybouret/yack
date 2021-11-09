
#include "yack/data/list/ordered.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/utest/run.hpp"
#include "yack/object.hpp"
#include <cstring>

using namespace yack;

namespace
{
    class Node : public object
    {
    public:
        explicit Node(const int c) : code(c), next(0), prev(0) {}
        virtual ~Node() throw() {}

        const int code;
        Node     *next;
        Node     *prev;

        static sign_type Compare(const int &code, const Node *node)
        {
            return __sign::of(code,node->code);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Node);
    };

    typedef cxx_list_of<Node> List;

}

YACK_UTEST(data_list_ordered)
{

    randomized::rand_ ran;

    int keys[100];
    const size_t knum = sizeof(keys)/sizeof(keys[0]);

    for(size_t i=0;i<knum;++i) keys[i] = int(i)*2;

    for(size_t iter=0;iter<16;++iter)
    {
        randomized::shuffle::data(keys,knum,ran);

        List L;
        for(size_t i=0;i<knum;++i)
        {
            const int key  = keys[i];
            Node     *node = NULL;
            YACK_ASSERT(!ordered_list::search(L,key,Node::Compare,node));
            if(node)
            {
                //std::cerr << "insert " << key << " after " << node->code << std::endl;
                L.insert_after(node, new Node(key) );
            }
            else
            {
                //std::cerr << "push front " << key << std::endl;
                L.push_front( new Node(key) );
            }
        }

        for(const Node *node=L.head;node->next;node=node->next)
        {
            assert(node->code<node->next->code);
        }


        for(size_t i=0;i<2*knum;++i)
        {
            const int key  = int(i);
            Node     *node = NULL;
            if(0==(i&1))
            {
                YACK_ASSERT(ordered_list::search(L,key,Node::Compare,node));
                YACK_ASSERT(node);
                YACK_ASSERT(key==node->code);
            }
            else
            {
                YACK_ASSERT(!ordered_list::search(L,key,Node::Compare,node));
            }
        }


    }

}
YACK_UDONE()

