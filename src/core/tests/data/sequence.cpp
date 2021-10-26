#include "yack/sequence/list.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_sequence)
{
    randomized::rand_ ran;

    list<int> il;
    il.reserve(20);

    for(size_t i=100;i>0;--i)
    {
        const int j = ran.full<uint16_t>();
        if(ran.choice())
        {
            il.push_back(j);
        }
        else
        {
            il.push_front(j);
        }
    }
    il.reserve(10);
    il.free();
    

}
YACK_UDONE()


