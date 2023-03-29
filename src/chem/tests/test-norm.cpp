#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/realm/normalizer.hpp"
#include "yack/chem/realm.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;
using namespace math;

YACK_UTEST(norm)
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
    realm      chem(lib,eqs,K);
    normalizer norm(chem);

    chem.graphviz("cs",1);

    const size_t M = lib->size;
    const size_t N = eqs->size;
    vector<double>     C0(M,0), C1(M,0);
    vector<double>     xi(N,0);
    cameo::add<double> xadd;

    for(size_t i=N;i>0;--i) xi[i]= library::conc(ran);
    std::cerr << "xi=" << xi << std::endl;

    iota::mul(C0,chem.NuT,xi,xadd);
    iota::load(C1,C0);

    norm(chem,C1);
    lib(std::cerr << "C0=",C0) << std::endl;
    lib(std::cerr << "C1=",C1) << std::endl;
    
    


    
    

}
YACK_UDONE()

