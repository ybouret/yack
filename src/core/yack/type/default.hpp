
//! \file
#ifndef YACK_TYPE_DEFAULT_INCLUDED
#define YACK_TYPE_DEFAULT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack {

    typedef type2type<void>     default_type_t; //!< helper to use default functions
    extern const default_type_t default_type;   //!< syntax helper

}

#endif
