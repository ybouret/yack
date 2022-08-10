#include "yack/chem/forge.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(components)
{
    randomized::rand_ ran;

    library lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",-1);

    std::cerr << lib << std::endl;

    components water;
    water(h,1);
    water(w,1);
    water.display(std::cerr) << std::endl;

    chemical::forge &build = chemical::forge::instance();

    if(argc>1)
    {
        components sample;
        build(sample,lib,argv[1]);
        sample.display(std::cerr) << std::endl;
    }

    std::cerr << lib << std::endl;
    
    vector<double> C(lib.size(),0);

    lib.fill(C,0.8,ran);

    lib(std::cerr,C);



}
YACK_UDONE()
