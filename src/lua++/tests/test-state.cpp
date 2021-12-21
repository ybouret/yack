#include "yack/lua++/state.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(state)
{
    Lua::VM vm = new Lua::State();
}
YACK_UDONE()
