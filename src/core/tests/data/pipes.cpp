#include "yack/pipe/stack.hpp"
#include "yack/pipe/queue.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"

using namespace yack;


YACK_UTEST(data_pipes)
{
    randomized::rand_ ran;

    stack< int,vector<int>  > ivs;
    stack< int,list<int>    > ils;
    queue< int, vector<int> > ivq;
    queue< int, list<int>   > ilq;


    for(size_t i=2+ran.leq(10);i>0;--i)
    {
        const int j = ran.to<uint16_t>();
        ivs.push(j);
        ils.push(j);
        ivq.push(j);
        ilq.push(j);

        std::cerr << "peek: " << ivs.peek() << " " << ils.peek() << " " << ivq.peek() << " " << ilq.peek() << std::endl;

    }

    std::cerr << ivs << std::endl;
    std::cerr << ils << std::endl;
    std::cerr << ivq << std::endl;
    std::cerr << ilq << std::endl;
    
}
YACK_UDONE()


