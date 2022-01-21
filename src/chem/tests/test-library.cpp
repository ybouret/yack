
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(library)
{
    YACK_SIZEOF(library);

    library        lib;
    const species &proton = lib("H+",1);
    std::cerr << proton << std::endl;

}
YACK_UDONE()
