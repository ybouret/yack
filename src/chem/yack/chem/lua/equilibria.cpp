#include "yack/chem/lua/equilibria.hpp"

namespace yack
{
    namespace chemical
    {

        lua_equilibria:: ~lua_equilibria() noexcept
        {
        }

        lua_equilibria:: lua_equilibria() : equilibria(), vm( new Lua::State() )
        {
        }
        

    }

}

