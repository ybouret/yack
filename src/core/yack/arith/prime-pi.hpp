
//! \file

#ifndef YACK_ARITH_PRIME_PI_INCLUDED
#define YACK_ARITH_PRIME_PI_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace core
    {

        //! pre-computed PrimePi function for 2^n
        struct prime_pi
        {
            static const uint64_t table[]; //!< PrimePi(2^n)
            static const size_t   count;   //!< nmax
        };
    }

}

#endif
