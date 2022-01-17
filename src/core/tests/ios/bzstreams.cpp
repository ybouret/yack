
#include "yack/ios/bz/istream.hpp"
#include "yack/ios/bz/ostream.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

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
    else
    {
        const string     fn = "data.data.bz2";
        //ios::bz::ostream fp(fn);
    }
}
YACK_UDONE()

