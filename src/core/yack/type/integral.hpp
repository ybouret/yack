//! \file

#ifndef YACK_TYPE_INTEGRAL_INCLUDED
#define YACK_TYPE_INTEGRAL_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/check/static.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace core
    {
        template <typename TARGET, typename SOURCE>
        struct uu_integral {

            static inline TARGET convert(const SOURCE source, const char *ctx)
            {
                static const int2type<sizeof(SOURCE)<=sizeof(TARGET)> chk = {};
                return _(source,ctx,chk);
            }

        private:
            // sizeof(SOURCE)<=sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *,
                                   const int2type<true> &) throw()
            {
                YACK_STATIC_CHECK(!is_signed<TARGET>::value,signed_type);
                YACK_STATIC_CHECK(sizeof(SOURCE)<=sizeof(TARGET),SOURCE_too_big);
                return TARGET(u);
            }

            // sizeof(SOURCE)>sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *ctx,
                                   const int2type<false> &)
            {
                YACK_STATIC_CHECK(!is_signed<TARGET>::value,signed_type);
                YACK_STATIC_CHECK(sizeof(SOURCE)>sizeof(TARGET),SOURCE_too_small);
                throw libc::exception(EINVAL,"not implemented for %s", (ctx?ctx:yack_unknown));
                return TARGET(u);
            }

        };







    }

}

#endif

