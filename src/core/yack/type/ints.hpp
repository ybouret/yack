//! \file

#ifndef YACK_TYPE_INTS_INCLUDED
#define YACK_TYPE_INTS_INCLUDED 1

#include "yack/type/pick.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! compute unsigned int of a given size
    //__________________________________________________________________________
    template <const size_t N>
    struct unsigned_int ;

    //__________________________________________________________________________
    //
    //! will be uint8_t
    //__________________________________________________________________________
    template <>
    struct unsigned_int<1> {
        typedef uint8_t      type;            //!< mapping
        static const uint8_t minimum = 0;     //!< min
        static const uint8_t maximum = 0xff;  //!< max
        //! check
        static inline type   positive(const type x) throw() { return x; }
    };

    //__________________________________________________________________________
    //
    //! will be uint16_t
    //__________________________________________________________________________
    template <>
    struct unsigned_int<2> {
        typedef uint16_t      type;              //!< mapping
        static const uint16_t minimum = 0;       //!< min
        static const uint16_t maximum = 0xffff;  //!< max

        //! check
        static inline type    positive(const type x) throw() { return x; }
    };

    //__________________________________________________________________________
    //
    //! will be uint32_t
    //__________________________________________________________________________
    template <>
    struct unsigned_int<4> {
        typedef uint32_t      type;                 //!< mapping
        static const uint32_t minimum = 0;          //!< min
        static const uint32_t maximum = 0xffffffff; //!< max

        //! check
        static inline type    positive(const type x) throw() { return x; }
    };

    //__________________________________________________________________________
    //
    //! will be uint64_t
    //__________________________________________________________________________
    template <>
    struct unsigned_int<8> {
        typedef uint64_t type;                                     //!< mapping
        static const uint64_t minimum = 0;                         //!< min
        static const uint64_t maximum = YACK_U64(0xffffffffffffffff); //!< max

        //! check
        static inline type    positive(const type x) throw() { return x; }
    };

    //! matching unsigned type
    template <typename T>
    struct unsigned_for
    {
        typedef typename unsigned_int<sizeof(T)>::type type; //!< matching type
    };


    //__________________________________________________________________________
    //
    //! compute signed int of a given size
    //__________________________________________________________________________
    template <const size_t N>
    struct signed_int;

    //__________________________________________________________________________
    //
    //! will be int8_t
    //__________________________________________________________________________
    template <>
    struct signed_int<1> {
        typedef int8_t      type;                    //!< mapping
        static const int8_t maximum =  127;          //!< min
        static const int8_t minimum = (-maximum-1);  //!< max

        //! check
        static inline type  positive(const type x) throw() { return (x<0)?0:x; }
    };

    //__________________________________________________________________________
    //
    //! will be int16_t
    //__________________________________________________________________________
    template <>
    struct signed_int<2> {
        typedef int16_t      type;                     //!< mapping
        static const int16_t maximum =  32767;         //!< max
        static const int16_t minimum = (-maximum-1);   //!< min

        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };

    //__________________________________________________________________________
    //
    //! will be int32_t
    //__________________________________________________________________________
    template <>
    struct signed_int<4> {
        typedef int32_t      type;                   //!< mapping
        static const int32_t maximum =  2147483647;  //!< max
        static const int32_t minimum = (-maximum-1); //!< min

        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };

    //__________________________________________________________________________
    //
    //! will be int64_t
    //__________________________________________________________________________
    template <>
    struct signed_int<8> {
        typedef int64_t      type;                                    //!< mapping
        static const int64_t maximum =  YACK_I64(0x7fffffffffffffff); //!< max
        static const int64_t minimum =  (-maximum-1);                 //!< min

        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };

    //! matching signed type
    template <typename T>
    struct signed_for
    {
        typedef typename signed_int<sizeof(T)>::type type; //!< resulting type
    };
    

    //! compute if an integral type is signed
    template <typename T>
    struct is_signed {
        static const bool value = (T)-1 < 0; //!< compile time detection
    };

    //! deduce matching integral type
    template <typename T>
    struct integral_for
    {
        static const size_t _sizeof = sizeof(T);                               //!< sizeof(T)
        static const bool   _signed = is_signed<T>::value;                     //!< signedness
        typedef signed_int<_sizeof>                                  _s_int_t; //!< signed parent
        typedef unsigned_int<_sizeof>                                _u_int_t; //!< unsigned parent
        typedef typename pick_from<_signed,_s_int_t,_u_int_t>::type  parent;   //!< chose parent
        typedef typename parent::type                                type;     //!< matching type

        static const type minimum = parent::minimum; //!< alias
        static const type maximum = parent::maximum; //!< alias
    };

}

#endif

