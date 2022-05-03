
#include "yack/container/groove.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(data_groove)
{
    randomized::rand_ ran;
    groove g;

    for(size_t i=0;i<=1000;i+=10+ran.leq(100))
    {
        const size_t bs = 1+ran.leq(15);
        std::cerr << "ensure(" << i << "," << bs <<")" << std::endl;
        g.ensure(i,bs);
        std::cerr << "granted: " << g.granted() << " / " << i*bs << std::endl;
    }

    g.release();

    g.build<double>(4);
    g.build<string>();

    g.as<string>() = "Hello";
    g.build<string>(8);

    YACK_SIZEOF(groove);

}
YACK_UDONE()

