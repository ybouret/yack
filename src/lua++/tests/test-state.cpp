#include "yack/lua++/state.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(state)
{
    Lua::VM vm = new Lua::State();

    if(argc>1)
    {
        vm->dofile(argv[1]);

        const int    I = vm->get<lua_Integer>("I"); std::cerr << I << std::endl;
        const double F = vm->get<lua_Number>("F");  std::cerr << F << std::endl;
        const string S = vm->get<string>("S");      std::cerr << S << std::endl;

    }

}
YACK_UDONE()
