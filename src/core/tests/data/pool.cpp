

#include "yack/data/raw-pool.hpp"
#include "yack/data/cxx-pool.hpp"

#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{

    struct cNode
    {
        cNode    *next;
        size_t    data;
    };

    class xNode
    {
    public:
        xNode       *next;
        xNode       *prev;
        const size_t data;

        inline  xNode(const size_t i) throw() : next(0), prev(0), data(i) {}
        inline ~xNode() throw() {}
        inline  xNode(const xNode &other) throw() : next(0), prev(0), data(other.data) {}

    private:
        YACK_DISABLE_ASSIGN(xNode);
    };

}

YACK_UTEST(data_pool)
{
    uprng ran;

    {
        cNode nodes[1024];
        const size_t num = sizeof(nodes)/sizeof(nodes[0]);
        memset(nodes,0,sizeof(nodes));
        raw_pool_of<cNode> pool;
        for(size_t i=0;i<num;++i)
        {
            cNode *node = nodes+i;
            YACK_ASSERT(pool.owns( pool.store(node) ));
            pool.head->data = i;
        }

        pool.restart();
        memset(nodes,0,sizeof(nodes));

        for(size_t i=0;i<num;++i) pool.store(nodes+i)->data = i;
        ran.shuffle_pool(pool);
        while(pool.size)
        {
            cNode *node = pool.query();
            //std::cerr << node->data << '/';
            YACK_ASSERT(node->data==static_cast<size_t>(node-nodes));
        }
        std::cerr << std::endl;
    }

    {
        cxx_pool_of<xNode> pool;
        for(size_t i=100+ran.leq(1000);i>0;--i)
        {
            pool.store( new xNode(i) );
        }

        {
            const cxx_pool_of<xNode> temp(pool);
            YACK_CHECK(temp.size==pool.size);
            bool  sameData = true;
            for(const xNode *lhs=temp.head, *rhs=pool.head;lhs;lhs=lhs->next,rhs=rhs->next)
            {
                if(lhs->data!=rhs->data)
                {
                    sameData = false;
                    break;
                }
            }
            YACK_CHECK(sameData);
        }
    }

    YACK_SIZEOF(pool_of<xNode>);
    YACK_SIZEOF(raw_pool_of<xNode>);
    YACK_SIZEOF(cxx_pool_of<xNode>);
    YACK_SIZEOF(core_pool_of<xNode>);



}
YACK_UDONE()

