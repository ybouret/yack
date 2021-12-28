//! \file

#ifndef YACK_JIVE_SYNTAX_TERM_ROLE_INCLUDED
#define YACK_JIVE_SYNTAX_TERM_ROLE_INCLUDED 1


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            //! role for a terminal
            enum term_role
            {
                standard, //!< regular expression with mutliple matches
                univocal, //!< regular expression with only one match
                division  //!< division only terminal
            };

        }

    }
}

#endif
