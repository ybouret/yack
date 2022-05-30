
//! \file

#ifndef YACK_ARITH_PRIME_PI_INCLUDED
#define YACK_ARITH_PRIME_PI_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace core
    {
        
        struct prime_pi
        {
            static const uint64_t table[];
            static const size_t   count;
        };
    }

}

#endif
