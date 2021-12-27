
//! \file

#ifndef YACK_CXX_NAME_INCLUDED
#define YACK_CXX_NAME_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    //! create a cxx valid name
    struct cxx_name
    {
        static string of(const string &); //!< replace bad chars with underscore
        static string of(const char   *); //!< replace bad chars with underscore
    };

}

#endif
