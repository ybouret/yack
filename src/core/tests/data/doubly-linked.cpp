#include "yack/data/doubly-linked.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(data_doubly)
{

    typedef doubly_linked<string> dstring;

    dstring::list_type dsl;

    dsl.push_back( new dstring()              );
    dsl.push_back( new dstring("Hello")       );
    dsl.push_back( new dstring(5,as_capacity) );


}
YACK_UDONE()
