#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(library)
{
    library lib;
    lib("H+",1);
    lib("HO-",-1);

    std::cerr << lib << std::endl;
    

}
YACK_UDONE()
