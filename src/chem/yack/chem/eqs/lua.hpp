
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
        class luaEquilibria : public equilibria
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit luaEquilibria();          //!< setup with Lua::Sate
            virtual ~luaEquilibria() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(library &, const string &info); //!< parse species/equilibrium/named
            void operator()(library &, const char   *info); //!< parse helper

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Lua::VM vm;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(luaEquilibria);
            void create_eq_for(library &, vector<string> &word);
            void insert_eq_for(library &, const string   &rx);
            
        };

    }

}

#endif

