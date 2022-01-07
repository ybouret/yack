
//! \file

#ifndef YACK_JIVE_SYNTAX_AGG_ROLE_INCLUDED
#define YACK_JIVE_SYNTAX_AGG_ROLE_INCLUDED 1


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            //! role for an aggregate
            enum agg_role
            {
                named, //!< make a named internal
                group, //!< transient grouping
                proxy  //!< yield if single content
            };

        }

    }
}

#endif
