#include "yack/ios/c/writable.hpp"
#include "yack/ios/c/readable.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_c_file)
{
    size_t nw = 0;

    {
        ios::writable_file fp("c.dat",false);
        nw = fp.putf("Hello, World from line %d\n",__LINE__);
        fp.flush();
    }
    std::cerr << "written=" << nw << std::endl;

    {
        ios::readable_file fp("c.dat");
        size_t nr = 0;
        char   C  = 0;
        while( fp.get(C) )
        {
            std::cerr << C;
            ++nr;
        }
        YACK_CHECK(nw==nr);
    }
    

}
YACK_UDONE()

