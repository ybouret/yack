

#include "yack/chem/components.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(components)
{

    chemical::library    lib;
    chemical::components cmp;
    if(argc>1)
    {
        cmp(argv[1],lib);
    }
    std::cerr << lib << std::endl;
    std::cerr << cmp << std::endl;

}
YACK_UDONE()
