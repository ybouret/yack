

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/utest/run.hpp"
#include "../../core/tests/main.hpp"
#include "yack/chem/designer/cm-parser.hpp"
#include "yack/jive/syntax/xnode.hpp"

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

    rmulops ops;
    
    std::cerr << "comp  ma: " << comp.mass_action(pow(10,-4.8), C, ops) << std::endl;
    std::cerr << "water ma: " << water.mass_action(1e-14, C, ops) << std::endl;


    std::cerr << "comp  Z = " << comp.reac.algebraic_Z << " -> " << comp.prod.algebraic_Z << std::endl;
    std::cerr << "water Z = " << water.reac.algebraic_Z << " -> " << water.prod.algebraic_Z << std::endl;


    nucleus::cm_parser cmp;
    if(argc>1)
    {
        auto_ptr<jive::syntax::xnode> code = cmp.parse( jive::module::open_data(argv[1]) );
        if(code.is_valid())
        {
            code->gv("cm.dot");
        }
    }
    
}
YACK_UDONE()

