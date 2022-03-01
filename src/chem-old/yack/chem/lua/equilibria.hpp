

//! \file

#ifndef YACK_CHEM_LUA_EQS_INCLUDED
#define YACK_CHEM_LUA_EQS_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! advanced equilibria
        //
        //______________________________________________________________________
        class lua_equilibria : public equilibria, public Lua::VM
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~lua_equilibria() throw();
            explicit lua_equilibria();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! species/equilibria compiler
            /**

             */
            void operator()(const string &expr, library &lib);

            //! wrapper
            void operator()(const char   *expr, library &lib);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lua_equilibria);
        };

    }

}

#endif
