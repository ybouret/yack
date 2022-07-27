
#include "yack/ordered/list.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

 

YACK_UTEST(data_ordered_list)
{
    randomized::rand_ ran;
    
    {
        ordered_list<int> L;
        for(size_t i=10;i>0;--i)
        {
            L.insert( bring::get<int>(ran) );
            std::cerr << L << std::endl;
        }
    }
    
    {
        ordered_list<string> L;
        for(size_t i=10;i>0;--i)
        {
            L.insert( bring::get<string>(ran) );
            std::cerr << L << std::endl;
        }
    }
    
    
}
YACK_UDONE()

