
//! \file

#ifndef YACK_AQUEOUS_LUA_EQUILIBRIA_INCLUDED
#define YACK_AQUEOUS_LUA_EQUILIBRIA_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! equilibria with internal lua state
        //
        //______________________________________________________________________
        class lua_equilibria : public equilibria
        {
        public:
            explicit lua_equilibria();           //!< setup
            virtual ~lua_equilibria() noexcept;  //!< cleanup

            Lua::VM vm; //!< shared virtual machine

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lua_equilibria);
        };

    }

}

#endif

