

#include "yack/chem/components.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(components)
{

    chemical::library    lib;
    chemical::components comp;
    if(argc>1)
    {
        comp(argv[1],lib);
    }
}
YACK_UDONE()
