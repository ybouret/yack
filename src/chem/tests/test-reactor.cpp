#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
{
    randomized::rand_ ran;
    library           lib;
    luaEquilibria     eqs;


    eqs->dostring("function f(t) return 1.1+math.cos(t) end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    reactor cs(lib,eqs,0.0);

    vector<double> C(cs.M,0);
    lib(std::cerr << "Cini=", "", C);

    if( cs.solve(C) )
    {
        lib(std::cerr << "Cend=", "", C);
    }




}
YACK_UDONE()
