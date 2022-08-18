#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_ ran;
    library           lib;
    luaEquilibria     eqs;


    eqs->dostring("function f(t) return 1.1+math.cos(t) end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "Testing Reactor with" << std::endl;
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;
    std::cerr << std::endl;

    reactor cs(lib,eqs,0.0);

    vector<double> C(cs.M,0);
    lib(std::cerr << "Cini=", "", C);

    if( cs.solve(C) )
    {
        lib(std::cerr << "Cend=", "", C);
    }
    else
    {
        throw exception("couldn't solve zero phase state!!");
    }

    lib.fill(C,ran);
    if( cs.solve(C) )
    {
        
    }




}
YACK_UDONE()
