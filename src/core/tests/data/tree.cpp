#include "yack/data/tree/suffix.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include <typeinfo>

using namespace yack;

namespace
{

}

YACK_UTEST(data_tree)
{

    suffix_tree<apq,uint32_t> tree;

    const char *key  = "hello";
    const apq   value(1/2);

    tree.insert(value,key,1);
    



}
YACK_UDONE()

