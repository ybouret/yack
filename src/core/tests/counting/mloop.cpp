#include "yack/counting/loop-on.hpp"
#include "yack/utest/run.hpp"
#include "yack/container/matrix.hpp"

using namespace yack;

YACK_UTEST(counting_loop)
{

    {
        const int ini[] = { 1, 3, 4, -1};
        const int end[] = { 2, 3, 0,  5};

        mloop<int> loop(ini,end,4);
        loop.display();

        std::cerr << "loop=" << loop << std::endl;
        std::cerr << std::endl;

        loop.boot();
        std::cerr << loop << std::endl;
        while( loop.next() )
        {
            std::cerr  << loop << std::endl;
        }

        matrix<int>    frame(loop.total,loop.size());
        cardinality_t  count =0;
        loop.boot();
        do
        {
            ++count;
            for(size_t j=loop.size();j>0;--j)
            {
                frame[loop.index][j] = loop[j];
            }

            for(size_t i=loop.index-1;i>0;--i)
            {
                if(!(frame[i] != loop))
                {
                    throw exception("multiple loop value");
                }
            }
            
        } while( loop.next() );

        YACK_ASSERT(loop.total==count);
    }



}
YACK_UDONE()

