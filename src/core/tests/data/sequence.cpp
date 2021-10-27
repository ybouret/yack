#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_sequence)
{
    randomized::rand_ ran;

    std::cerr << "Testing List" << std::endl;

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

    std::cerr << "Testing Vector" << std::endl;
    {vector<int> iv;}
    {
        vector<int> iv(10,as_capacity);
        std::cerr << "iv.capa=" << iv.capacity() << std::endl;
    }

    size_t capa = 0;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << capa << " => ";
        capa = container::next_capacity(capa);
        std::cerr << capa << std::endl;
    }


}
YACK_UDONE()


