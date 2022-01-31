
//! \file

#ifndef YACK_CHEM_BUILDER_INCLUDED
#define YACK_CHEM_BUILDER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/singleton.hpp"
#include "yack/jive/parser.hpp"
#include "yack/hashing/perfect.hpp"

namespace yack
{
    namespace Lua
    {
        class State;
    };

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! builder and database
        //
        //______________________________________________________________________
        class builder : public singleton<builder>
        {
        public:
            static const at_exit::longevity life_time = 0; //!< life time
            static const char               call_sign[];   //!< call sign
            typedef jive::syntax::xnode     xnode;         //!< alias
            

            xnode       *ast(const string &expr);                           //!< from species to equations...
            species     *ast_to_species(const xnode &tree) const;           //!< from a single species node
            species     *compile(const string &expr);                       //!< string to single species
            void         compile(equilibrium &, const string &, library &); //!< string to components for equilibrium
            void         compile(equilibria  &, const string &, library &, Lua::State &); //!< query database


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(builder);
            friend class singleton<builder>;
            explicit builder();
            virtual ~builder() throw();

            auto_ptr<jive::parser> P; //!< components parser
            const hashing::perfect H; //!< key words hasher



        };

    }

}

#endif
