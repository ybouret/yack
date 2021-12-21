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
        //______________________________________________________________________
        //
        //
        //! wrapper for lua_State
        //
        //______________________________________________________________________
        class State : public object, public counted
        {
        public:
            virtual ~State() throw(); //!< lua_close
            explicit State();         //!< lua_newstate

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(State);
            lua_State *L;
        };

        //______________________________________________________________________
        //
        //
        //! shared lua state
        //
        //______________________________________________________________________
        typedef arc_ptr<State> VM;
    }
}

#endif

