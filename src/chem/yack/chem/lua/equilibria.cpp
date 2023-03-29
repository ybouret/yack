
#include "yack/aqueous/lua/equilibria.hpp"

namespace yack
{
    namespace aqueous
    {

        lua_equilibria:: ~lua_equilibria() noexcept
        {
        }

        lua_equilibria:: lua_equilibria() : equilibria(), vm( new Lua::State() )
        {
        }
        

    }

}

