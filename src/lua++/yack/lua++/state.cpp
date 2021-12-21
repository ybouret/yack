
#include "yack/lua++/state.hpp"

#include "yack/exception.hpp"

namespace yack
{
    namespace Lua
    {
        State:: ~State() throw()
        {
            lua_close(L);
        }

        State:: State() :
        L( luaL_newstate() )
        {
            if(!L) throw exception("luaL_newtstate failure");
            luaL_openlibs(L);
        }

    }
}
