//! \file

#ifndef YACK_TYPE_INTEGRAL_INCLUDED
#define YACK_TYPE_INTEGRAL_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/check/static.hpp"

namespace yack
{

    template <typename TARGET, typename SOURCE>
    struct uu_integral {
        static TARGET convert(const SOURCE, const char *);
    };

    template <typename TARGET>
    struct uu_integral<TARGET, TARGET>
    {
        static inline TARGET convert(const TARGET u, const char *) throw()
        {
            YACK_STATIC_CHECK(!is_signed<TARGET>::value,signed_type);
            return u;
        }
    };

}

#endif

