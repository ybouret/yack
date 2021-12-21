//! \file

#ifndef YACK_LUAXX_STATE_INCLUDED
#define YACK_LUAXX_STATE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/lua/lua.h"

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

        };

    }
}

#endif

