#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/realm/normalizer.hpp"
#include "yack/aqueous/realm.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

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
    realm chem(lib,eqs,K);

    const size_t M = lib->size;
    const size_t N = eqs->size;
    vector<double> C(M,0);
    vector<double> xi(N,0);
    cameo::add<double> xadd;
    for(size_t i=N;i>0;--i) xi[i]= ran.symm<double>();

    chem.add(C,xi,xadd);
    


    
    

}
YACK_UDONE()

