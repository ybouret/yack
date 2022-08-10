

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


YACK_UTEST(components)
{
    
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

    
}
YACK_UDONE()

