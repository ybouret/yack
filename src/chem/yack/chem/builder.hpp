
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

        class builder : public singleton<builder>
        {
        public:
            static const at_exit::longevity life_time = 0; //!< life time
            static const char               call_sign[];   //!< call sign

            species * parse_species(const string &);
            

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
