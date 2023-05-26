
//! \file

#ifndef YACK_CHEM_LUA_EQUILIBRIA_INCLUDED
#define YACK_CHEM_LUA_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace Chemical
    {

        class LuaEquilibria : public Equilibria, public Lua::VM
        {
        public:
            explicit LuaEquilibria();
            virtual ~LuaEquilibria() noexcept;

            Equilibrium & operator()(const string &name, const string &kstr);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };

    }

}

#endif

