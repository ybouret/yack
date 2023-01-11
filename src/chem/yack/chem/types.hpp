
//! \file

#ifndef YACK_CHEMICAL_TYPES_INCLUDED
#define YACK_CHEMICAL_TYPES_INCLUDED 1

#include "yack/math/multiplier.hpp"
#include "yack/math/adder.hpp"

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
        typedef math::adder<double>      raddops; //!< alias
    }
}

#endif

