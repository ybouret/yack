//! \file

#ifndef YACK_JIVE_TYPES_INCLUDED
#define YACK_JIVE_TYPES_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace jive
    {

        typedef ark_ptr<string,string> tag;     //!< shared string
        typedef suffix_set<string,tag> tags_;   //!< base class for database
    }
}

#endif

