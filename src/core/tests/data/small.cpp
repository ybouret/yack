

#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/data/small/list.hpp"
#include "yack/data/small/pool.hpp"

#include "yack/utest/run.hpp"
#include "yack/system/offset.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(data_small)
{

    small_list<int> il;

    for(int i=0;i<=10;++i)
    {
        il.append_back(i);
        il.append_front(-i);
    }

    for(small_list<int>::node_type *node=il.head;node;node=node->next)
    {
        std::cerr << "/" << **node;
    }
    std::cerr << std::endl;


}
YACK_UDONE()

