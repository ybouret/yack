#include "yack/data/joint.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace {

    struct xNode
    {

        xNode *next;
        xNode *prev;
    };

    typedef joint_pool<xNode> xPool;
    typedef joint_list<xNode> xList;

}

YACK_UTEST(data_joint)
{
    YACK_SIZEOF(joint_pool<xNode>);
    YACK_SIZEOF(joint_list<xNode>);

    xPool::pointer fund = new xPool();

    fund->reserve(10);

}
YACK_UDONE()

