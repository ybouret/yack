
//! \file

#ifndef YACK_CHEMICAL_TYPES_INCLUDED
#define YACK_CHEMICAL_TYPES_INCLUDED 1

#include "yack/cameo/mul.hpp"
#include "yack/cameo/add.hpp"
#include "yack/proto-ostream.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // global definitions
        //
        //______________________________________________________________________
        typedef cameo::mul<double> rmulops; //!< alias
        typedef cameo::add<double> raddops; //!< alias
    }
}

#endif

