
#include "yack/chem/eqs/lua.hpp"

namespace yack
{
    namespace chemical
    {

        luaEquilibria:: ~luaEquilibria() throw()
        {
        }

        luaEquilibria:: luaEquilibria() : equilibria(), Lua::VM( new Lua::State() )
        {
        }
        
    }

}
