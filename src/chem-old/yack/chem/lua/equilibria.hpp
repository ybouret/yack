
//! \file

#ifndef YACK_CHEM_LUA_EQUILIBRIA_INCLUDED
#define YACK_CHEM_LUA_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"


namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! equilibria with a shared virtual machine
        //
        //______________________________________________________________________
        class lua_equilibria : public equilibria, public Lua::VM
        {
        public:
            explicit lua_equilibria();          //!< setup
            virtual ~lua_equilibria() throw();  //!< cleanup

            void operator()(library &, const string &data); //!< parse
            void operator()(library &, const char   *data); //!< parse


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lua_equilibria);
        };

    }
}

#endif
