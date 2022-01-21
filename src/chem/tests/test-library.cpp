
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

    if(argc>1)
    {
        const string   expr = argv[1];
        const species &sp   = lib(expr);
        std::cerr << "species=" << sp << std::endl;
    }
    
}
YACK_UDONE()
