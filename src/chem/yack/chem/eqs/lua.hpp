
//! \file

#ifndef YACK_CHEMICAL_EQS_LUA_INCLUDED
#define YACK_CHEMICAL_EQS_LUA_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace chemical
    {

        class luaEquilibria : public equilibria, public Lua::VM
        {
        public:
            explicit luaEquilibria();
            virtual ~luaEquilibria() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(luaEquilibria);

        };

    }

}

#endif

