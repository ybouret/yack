
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/realm.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(realm)
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
    realm cs(lib,eqs,K);



}
YACK_UDONE()

