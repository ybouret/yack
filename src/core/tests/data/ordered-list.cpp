
#include "yack/ordered/list.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

 

YACK_UTEST(data_sorted_list)
{
    randomized::rand_ ran;
    
    {
        sorted_list<int> L;
        for(size_t i=10;i>0;--i)
        {
            L.insert( bring::get<int>(ran) );
            std::cerr << L << std::endl;
        }
        std::cerr << std::endl;
    }
    
    {
        sorted_list<string> L;
        for(size_t i=10;i>0;--i)
        {
            L.insert( bring::get<string>(ran) );
            std::cerr << L << std::endl;
        }
        std::cerr << std::endl;
    }
    
    {
        sorted_list<double> L;
        L.insert(2);
        L.insert(1);
        std::cerr << L << std::endl;
        L.insert(0.5,3);
        std::cerr << L << std::endl;
        L.insert(1.5,4);
        std::cerr << L << std::endl;
        L.insert(2.1,5);
        std::cerr << L << std::endl;

    }
    
    
}
YACK_UDONE()

