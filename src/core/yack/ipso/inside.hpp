
//! \file

#ifndef YACK_IPSO_INSIDE_INCLUDED
#define YACK_IPSO_INSIDE_INCLUDED 1

#include "yack/math/extended.hpp"

namespace yack
{
    namespace ipso
    {
        //! default for apex types
        template <typename T>
        struct inside
        {
            typedef T type;
            static inline const type & send(const T    &args) noexcept { return args; }
            static inline const T    & recv(const type &args) noexcept { return args; }
            static inline sign_type    comp(const type &l, const type &r) {
                return l<r ? negative : (r<l ? positive : __zero__);
            }
        };

        //! extended reals ops
        template <typename XREAL>
        struct xr_ops
        {
            static inline sign_type compare(const XREAL &l,
                                            const XREAL &r) noexcept
            {
                return __sign::of(l.p,r.p);
            }
        };

        template <>
        struct inside<float>
        {
            typedef extended<float> type;
            static inline type      send(const float args)   { return  args; }
            static inline float     recv(const type &args)   { return *args; }
            static inline sign_type comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }

        };

        template <>
        struct inside<double>
        {
            typedef extended<double> type;
            static inline type       send(const double args) { return  args; }
            static inline double     recv(const type  &args) { return *args; }
            static inline sign_type  comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }
        };


        template <>
        struct inside<long double>
        {
            typedef extended<long double> type;
            static inline type            send(const long double args) { return  args; }
            static inline long double     recv(const type       &args) { return *args; }
            static inline sign_type       comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }
        };

        
    }

}

#endif
