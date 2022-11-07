#include "yack/chem/outcome.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(balance)
{

    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;
    rmulops            xmul;
    raddops            xadd;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);


    }

    std::cerr << "[[ LIBRARY ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    const size_t   M = lib.size();
    vector<double> Corg(M,0);
    vector<double> Cend(M,0);
    vector<double> psi(M,0);
    vector<int>    nu(M,0);

    lib.fill(Corg,ran);
    std::cerr << "[[ Corg ]]" << std::endl;
    lib(std::cerr,"",Corg);
    std::cerr << std::endl;

}
YACK_UDONE()
