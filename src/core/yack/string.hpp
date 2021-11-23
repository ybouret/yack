
//! \file

#ifndef YACK_STRING_INCLUDED
#define YACK_STRING_INCLUDED 1

#include "yack/string/string.hpp"
#include "yack/check/printf.hpp"

namespace yack
{
    typedef kernel::string<char> string; //!< alias

    //! vformat
    string vformat(const char *fmt,...) YACK_PRINTF_CHECK(1,2);

}

#endif


