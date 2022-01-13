
#include "yack/ios/bz/istream.hpp"
//#include "yack/ios/gz/ostream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    
}

YACK_UTEST(ios_bz)
{
    if(argc>1)
    {
        ios::bz::istream fp( argv[1] );
        char C=0;
        while(fp.query(C))
        {
            std::cerr << C;
        }
    }
}
YACK_UDONE()

