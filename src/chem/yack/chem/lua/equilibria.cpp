

#include "yack/chem/lua/equilibria.hpp"

namespace yack
{
    namespace chemical
    {

        lua_equilibria:: ~lua_equilibria() throw() {}

        lua_equilibria:: lua_equilibria() :
        equilibria(),
        Lua::VM( new Lua::State() )
        {
        }
        
        

    }

}


