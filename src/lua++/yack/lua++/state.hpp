//! \file

#ifndef YACK_LUAXX_STATE_INCLUDED
#define YACK_LUAXX_STATE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/arc.hpp"
extern "C"
{
#include "yack/lua/lua.h"
#include "yack/lua/lualib.h"
#include "yack/lua/lauxlib.h"
}

namespace yack
{
    namespace Lua
    {
        class State : public object, public counted
        {
        public:
            virtual ~State() throw();
            explicit State();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(State);
            lua_State *L;
        };

        typedef arc_ptr<State> VM;
    }
}

#endif

