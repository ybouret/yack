
//! \file

#ifndef YACK_CHEMICAL_TYPES_INCLUDED
#define YACK_CHEMICAL_TYPES_INCLUDED 1

#include "yack/math/multiplier.hpp"
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
        typedef math::multiplier<double> rmulops; //!< alias
        typedef cameo::add<double>       raddops; //!< alias
    }
}

#endif

