
#include "yack/chem/forge.hpp"

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

    chemical::forge &build = chemical::forge::instance();

    if(argc>1)
    {
        components sample;
        build(sample,lib,argv[1]);
    }





}
YACK_UDONE()
