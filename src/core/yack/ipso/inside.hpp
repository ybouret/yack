//! \file

#ifndef YACK_IPSO_INSIDE_INCLUDED
#define YACK_IPSO_INSIDE_INCLUDED 1

#include "yack/math/xcomplex.hpp"

namespace yack
{
    namespace ipso
    {
        //______________________________________________________________________
        //
        //! default for apex types
        //______________________________________________________________________
        template <typename T>
        struct inside
        {
            typedef T type; //!< alias
            static inline const type & send(const T    &args) noexcept { return args; } //!< Id
            static inline const T    & recv(const type &args) noexcept { return args; } //!< Id
            static inline sign_type    comp(const type &l, const type &r) {
                return l<r ? negative : (r<l ? positive : __zero__);
            } //!< default comparison
        };

    }

    namespace ipso
    {
        //! extended reals ops
        template <typename XREAL>
        struct xr_ops
        {
            //! compare by increasing exponent
            static inline sign_type compare(const XREAL &l, const XREAL &r) noexcept {
                return __sign::of(l.p,r.p);
            }
        };

        //______________________________________________________________________
        //
        //! float / extended
        //______________________________________________________________________
        template <>
        struct inside<float>
        {
            typedef extended<float> type; //!< alias
            static inline type      send(const float args)   { return  args; } //!< flt -> xflt
            static inline float     recv(const type &args)   { return *args; } //!< xflt -> flt
            static inline sign_type comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            } //!< comparison for mul

        };

        //______________________________________________________________________
        //
        //! double / extended
        //______________________________________________________________________
        template <>
        struct inside<double>
        {
            typedef extended<double> type; //!< aias
            static inline type       send(const double args) { return  args; } //!< dbl -> xdbl
            static inline double     recv(const type  &args) { return *args; } //!< xdbl -> dbls
            static inline sign_type  comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            } //!< comparison for mul
        };

        //______________________________________________________________________
        //
        //! long double / extended
        //______________________________________________________________________
        template <>
        struct inside<long double>
        {
            typedef extended<long double> type; //!< alias
            static inline type            send(const long double args) { return  args; } //!< ldbl -> xldbl
            static inline long double     recv(const type       &args) { return *args; } //!< xldbl -> ldbl
            static inline sign_type       comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            } //!< comparison for mul
        };

    }

    namespace ipso {

        //! extended reals ops
        template <typename XCPLX>
        struct xc_ops
        {
            //! compare by increasing exponent
            static inline sign_type compare(const XCPLX &l, const XCPLX &r) noexcept {
                const int lp = max_of(l.re.p,l.im.p);
                const int rp = max_of(r.re.p,r.im.p);
                return __sign::of(lp,rp);
            }
        };

        //______________________________________________________________________
        //
        //! complex<float> / xcomplex<float>
        //______________________________________________________________________
        template <>
        struct inside< complex<float> >
        {
            typedef xcomplex<float>      type; //!< alias
            static inline type           send(const complex<float> args)  { return  args; } //!< cplx -> xcplx
            static inline complex<float> recv(const type &args)           { return *args; } //!< xcplx-> cplx
            static inline sign_type      comp(const type &l, const type &r)
            {
                return xc_ops<type>::compare(l,r);
            } //!< comparison for mul
        };

        //______________________________________________________________________
        //
        //! complex<double> / xcomplex<double>
        //______________________________________________________________________
        template <>
        struct inside< complex<double> >
        {
            typedef xcomplex<double>      type; //!< alias
            static inline type            send(const complex<double> args)  { return  args; } //!< cplx -> xcplx
            static inline complex<double> recv(const type &args)           { return *args; } //!< xcplx-> cplx
            static inline sign_type       comp(const type &l, const type &r)
            {
                return xc_ops<type>::compare(l,r);
            } //!< comparison for mul
        };

        //______________________________________________________________________
        //
        //! complex<long double> / xcomplex<long double>
        //______________________________________________________________________
        template <>
        struct inside< complex<long double> >
        {
            typedef xcomplex<long double>      type; //!< alias
            static inline type                 send(const complex<long double> args)  { return  args; } //!< cplx -> xcplx
            static inline complex<long double> recv(const type &args)                 { return *args; } //!< xcplx-> cplx
            static inline sign_type            comp(const type &l, const type &r)
            {
                return xc_ops<type>::compare(l,r);
            } //!< comparison for mul
        };



    }

}

#endif
