
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_SP_PARSER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_SP_PARSER_INCLUDED 1


#include "yack/jive/parser.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! parser for species
            //
            //__________________________________________________________________
            class sp_parser : public jive::parser
            {
            public:
                virtual ~sp_parser() noexcept; //!< cleanup
                explicit sp_parser();         //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }
    }

}

#endif
