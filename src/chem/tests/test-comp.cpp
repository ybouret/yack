

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/utest/run.hpp"
#include "../../core/tests/main.hpp"

using namespace yack;
using namespace chemical;


YACK_UTEST(components)
{
    randomized::rand_ ran;
    
    library lib;
    lib << "H+:HO-:EtCOOH:EtCOO-";
    std::cerr << lib << std::endl;
    components comp;
    comp(lib["H+"],1);
    comp(lib["EtCOO-"],1);
    comp(lib["EtCOOH"],-1);
    std::cerr << lib << std::endl;

    components water;
    water(lib["H+"],1);
    water(lib["HO-"],1);
    std::cerr << lib << std::endl;
    
    std::cerr << "comp:  " << comp << std::endl;
    std::cerr << "water: " << water << std::endl;

    vector<double> C( lib.size(), 0);
    
    bring::fill(C,ran);
    std::cerr << "C=" << C << std::endl;
    lib(std::cerr << "C=", "", C);
    
    std::cerr << comp.genuine_limits(C,lib.maxlen)  << std::endl;
    std::cerr << water.genuine_limits(C,lib.maxlen) << std::endl;

}
YACK_UDONE()

