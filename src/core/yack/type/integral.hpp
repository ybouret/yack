//! \file

#ifndef YACK_TYPE_INTEGRAL_INCLUDED
#define YACK_TYPE_INTEGRAL_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{

    struct integral
    {
        template <typename TARGET, typename SOURCE> static TARGET convert(const SOURCE,const char *ctx);
    };
}

#endif

