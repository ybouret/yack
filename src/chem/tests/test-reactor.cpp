
#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/realm/normalizer.hpp"
#include "yack/chem/realm/engine.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;
using namespace math;

YACK_UTEST(reactor)
{
    randomized::rand_ ran;
    species::verbose = true;
    library            lib;
    lua_equilibria     eqs;
    weasel::designer  &wd = weasel::designer::instance();

    for(int i=1;i<argc;++i)
    {
        wd(lib,eqs,jive::module::open_data(argv[i]));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    vector<double> K;
    realm          chem(lib,eqs,K);

    
    normalizer     norm(chem);
    engine         eng(chem);

    chem.graphviz("cs",1);

    const size_t M = lib->size;
    //const size_t N = eqs->size;
    vector<double>     C0(M,0);
    lib.conc(C0,ran);
    for(size_t i=M;i>0;--i)
    {
        C0[i] = lib.conc(ran);
        if(ran.to<double>() >= 0.7) C0[i] = 0;
    }
    lib(std::cerr << "C0=",C0) << std::endl;


    chem.prepareK(K);
    chem.computeK(K,0.0);

    eng(chem,C0,K);
    lib(std::cerr << "C0=",C0) << std::endl;


}
YACK_UDONE()

