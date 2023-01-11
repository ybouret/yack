//! \file

#ifndef YACK_STRING_PLURAL_INCLUDED
#define YACK_STRING_PLURAL_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //! helper with plural forms
    struct plural
    {
        static const char *s(const size_t) throw(); //!< ""  | "s"
        static const char *x(const size_t) throw(); //!< ""  | "x"
        static const char *a(const size_t) throw(); //!< "um"| "a"
    };
}

#endif

