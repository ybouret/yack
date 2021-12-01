//! \file

#ifndef YACK_JIVE_TYPES_INCLUDED
#define YACK_JIVE_TYPES_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ios/istream.hpp"

namespace yack
{
    namespace jive
    {

        typedef ark_ptr<string,string> tag;     //!< shared string
        typedef suffix_set<string,tag> tags_;   //!< base class for database
        typedef arc_ptr<ios::istream>  input;   //!< input class
    }
}

#endif

