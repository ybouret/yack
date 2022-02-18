#include "yack/data/list/meta.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_meta)
{
    int arr[] = { 1,2,3,4 };

    meta_list<int>       viml;
    meta_list<const int> ciml;

    for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
    {
        viml << &arr[i];
        ciml << &arr[i];
    }

    for(const meta_node<int> *node=viml.head;node;node=node->next)
    {
        YACK_ASSERT(node->is_valid());
        std::cerr << **node << std::endl;
    }


}
YACK_UDONE()

