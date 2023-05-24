#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace Chemical;

YACK_UTEST(lib)
{

    YACK_SIZEOF(Species);
    YACK_SIZEOF(Library);


    Library lib;

    const Species   &proton = lib("H+",1);
    std::cerr << lib->size << std::endl;

    std::cerr << proton << std::endl;

    for(const SNode *sn = lib->head; sn; sn=sn->next)
    {

    }

}
YACK_UDONE()

