//! \file

#ifndef YACK_OUT_OF_REACH_INCLUDED
#define YACK_OUT_OF_REACH_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! functions to avoid aliasing on POD types
    //
    //__________________________________________________________________________
    struct out_of_reach
    {
        static void       *address(void       *addr)                                 throw(); //!< same address
        static const void *address(const void *addr)                                 throw(); //!< same address
        static void       *zset(void *addr, const size_t size)                       throw(); //!< memset(addr,0,size)
        static void       *copy(void *target, const void *source, const size_t size) throw(); //!< memcpy(target,source,size)
        static void        move(void *target, const void *source, const size_t size) throw(); //!< memmove(target,source,size)
        static void        swap(void *target, void *source, size_t size)             throw(); //!< memswap(target,source,size)
        static unit_t      diff(const void *a, const void *b)                        throw(); //!< b-a
        static bool        is0(const void *addr, const size_t size)                  throw(); //!< check only zero
        static void       *shift(void *addr, const ptrdiff_t offset)                 throw(); //!< addr += offset
        static const void *shift(const void *addr, const ptrdiff_t offset)           throw(); //!< addr += offset, const

        //! helper with type casting
        template <typename T> static inline
        T *naught(T *obj) throw() { assert(NULL!=obj); return static_cast<T*>(zset(obj,sizeof(T))); }

        //! helper to shift addresses
        template <typename T> static inline T *haul(T *obj, const ptrdiff_t n) throw()
        {
            static const ptrdiff_t sz = sizeof(T);
            return static_cast<T*>(shift(obj,sz*n));
        }

        //! next object
        template <typename T> static inline T *next(T *obj) throw()
        {
            static const ptrdiff_t sz = sizeof(T);
            return static_cast<T*>(shift(obj,sz));

        }

        //! previous object
        template <typename T> static inline T *prev(T *obj) throw()
        {
            static const ptrdiff_t sz = -ptrdiff_t(sizeof(T));
            return static_cast<T*>(shift(obj,sz));
        }

    };

    //! static field zset
#define YACK_STATIC_ZSET(FIELD) yack::out_of_reach::zset(FIELD,sizeof(FIELD))

    //! out of reach type coercion
    template <typename T, typename U> inline
    T *coerce_cast(U *field) throw() { return static_cast<T*>(out_of_reach::address(field) ); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    const T *coerce_cast(const U *field) throw() { return static_cast<const T*>(out_of_reach::address(field) ); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    T &coerce_to(U &args) throw() { return *coerce_cast<T,U>(&args); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    const T &coerce_to(const U &args) throw() { return *coerce_cast<T,U>(&args); }
    
}

#endif
