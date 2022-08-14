
//! \file

#ifndef YACK_CHEMICAL_EQS_LUA_INCLUDED
#define YACK_CHEMICAL_EQS_LUA_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! equilibira with Lua parser/evaluator
        //
        //______________________________________________________________________
        class luaEquilibria : public equilibria, public Lua::VM
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit luaEquilibria();          //!< setup with Lua::Sate
            virtual ~luaEquilibria() throw();  //!< cleanup
            
            void operator()(library &, const string &info);
            void operator()(library &, const char   *info);

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(luaEquilibria);

        };

    }

}

#endif

