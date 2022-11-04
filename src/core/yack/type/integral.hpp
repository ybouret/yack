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
        struct u2u_integral {
            static const unsigned source_bits = 8 * sizeof(SOURCE);
            static const unsigned target_bits = 8 * sizeof(TARGET);

            static inline TARGET convert(const SOURCE source, const char *ctx)
            {
                YACK_STATIC_CHECK(!is_signed<TARGET>::value,SOURCE_is_signed);
                static const int2type<sizeof(SOURCE)<=sizeof(TARGET)> chk = {};
                return _(source,ctx,chk);
            }

        private:
            // sizeof(SOURCE)<=sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *,
                                   const int2type<true> &) throw()
            {
                YACK_STATIC_CHECK(sizeof(SOURCE)<=sizeof(TARGET),SOURCE_too_big);
                return TARGET(u);
            }

            // sizeof(SOURCE)>sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *ctx,
                                   const int2type<false> &)
            {
                YACK_STATIC_CHECK(sizeof(SOURCE)>sizeof(TARGET),SOURCE_too_small);
                static const SOURCE m( integral_for<TARGET>::maximum );
                if(u>m)
                    throw libc::exception(EDOM,"uint%u %s -> uint%u", source_bits, (ctx?ctx:yack_unknown), target_bits);
                return TARGET(u);
            }
        };



        //! signed to unsigned
        template <typename TARGET, typename SOURCE>
        struct s2u_integral {
            static const unsigned source_bits = 8 * sizeof(SOURCE);
            static const unsigned target_bits = 8 * sizeof(TARGET);

            static inline TARGET convert(const SOURCE source, const char *ctx)
            {
                YACK_STATIC_CHECK(is_signed<TARGET>::value,SOURCE_is_unsigned);
                if(source<0) throw libc::exception(EINVAL, "int%u %s < 0 for uint%u", source_bits, (ctx?ctx:yack_unknown), target_bits);
                static const int2type<sizeof(SOURCE)<=sizeof(TARGET)> chk = {};
                return _(source,ctx,chk);
            }

        private:
            // sizeof(SOURCE)<=sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *,
                                   const int2type<true> &) throw()
            {
                YACK_STATIC_CHECK(sizeof(SOURCE)<=sizeof(TARGET),SOURCE_too_big);
                assert(u>=0);
                return TARGET(u);
            }

            // sizeof(SOURCE)>sizeof(TARGET)
            static inline TARGET _(const SOURCE         u,
                                   const char           *ctx,
                                   const int2type<false> &)
            {
                YACK_STATIC_CHECK(sizeof(SOURCE)>sizeof(TARGET),SOURCE_too_small);
                static const SOURCE m( integral_for<TARGET>::maximum );
                if(u>m)
                    throw libc::exception(EINVAL,"int%u %s -> uint%u", source_bits, (ctx?ctx:yack_unknown), target_bits);
                return TARGET(u);
            }

        };




    }

}

#endif

