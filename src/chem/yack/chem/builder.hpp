
//! \file

#ifndef YACK_CHEM_BUILDER_INCLUDED
#define YACK_CHEM_BUILDER_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/singleton.hpp"
#include "yack/jive/parser.hpp"

namespace yack
{

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

            //! from species to equations...
            xnode *compile(const string &expr);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(builder);
            friend class singleton<builder>;
            explicit builder();
            virtual ~builder() throw();

            auto_ptr<jive::parser> eq;


        };

    }

}

#endif
