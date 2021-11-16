#include "yack/container/iterator/linear.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(data_iter)
{
    std::cerr << iterating::direction_text(iterating::forward) << std::endl;
    std::cerr << iterating::direction_text(iterating::reverse) << std::endl;


    int data[] = { 1, 2, 3, 4, 5 };
    {
        {
            const iterating::linear<int,iterating::forward> end(data+5);
            iterating::linear<int,iterating::forward>       cur(data+0);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }

        {
            const iterating::linear<const int,iterating::forward> end(data+5);
            iterating::linear<const int,iterating::forward>       cur(data+0);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }

        {
            const iterating::linear<int,iterating::reverse> end(data-1);
            iterating::linear<int,iterating::reverse>       cur(data+4);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }

        {
            const iterating::linear<const int,iterating::reverse> end(data-1);
            iterating::linear<const int,iterating::reverse>       cur(data+4);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }

    }


}
YACK_UDONE()


