
#include "yack/ios/ocstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_streams)
{

    ios::ocstream::overwrite("test.dat");
    ios::ocstream::echo("test.dat", "Hello from line %d\n", __LINE__);
    ios::ocstream::echo("test.dat", "World from line %d\n", __LINE__);

    {
        ios::ocstream fp("test2.dat");
        fp << 'a' << 'b' << 'c' << " def" << '\n';
        fp("result=%.3g\n", 3.14);
    }


}
YACK_UDONE()

