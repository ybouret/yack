
//! \file

#ifndef YACK_AQUEOUS_LUA_EQUILIBRIA_INCLUDED
#define YACK_AQUEOUS_LUA_EQUILIBRIA_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace aqueous
    {

        class lua_equilibria : public equilibria
        {
        public:
            explicit lua_equilibria();
            virtual ~lua_equilibria() noexcept;

            Lua::VM vm;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lua_equilibria);
        };

    }

}

#endif

