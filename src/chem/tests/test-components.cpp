
#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(components)
{
    library lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",-1);

    std::cerr << lib << std::endl;

    components water;
    water.add(h,1);
    water.add(w,1);
    water.display(std::cerr) << std::endl;



}
YACK_UDONE()
