#include "yack/ios/pstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_pstream)
{
    ios::pstream::result retval = new int;
    if(argc>1)
    {
        {
            ios::pstream fp(argv[1],retval);
        }
        
    }
}
YACK_UDONE()
