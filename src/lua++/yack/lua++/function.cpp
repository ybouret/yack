#include "yack/lua++/function.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace Lua
    {
        Function_:: ~Function_() throw()
        {
        }

        Function_:: Function_(const Function_ &F) :
        VM(F),
        name(F.name)
        {
        }
        
        void Function_:: fetch()
        {
            lua_State *L = ***this;
            lua_settop(L,0);
            lua_getglobal(L,name());
        }

    }

}

