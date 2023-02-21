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
        static void       *address(void       *addr)                                 noexcept; //!< same address
        static const void *address(const void *addr)                                 noexcept; //!< same address
        static void       *zset(void *addr, const size_t size)                       noexcept; //!< memset(addr,0,size)
        static void       *copy(void *target, const void *source, const size_t size) noexcept; //!< memcpy(target,source,size)
        static void        move(void *target, const void *source, const size_t size) noexcept; //!< memmove(target,source,size)
        static void        zmov(void *target, void *source, const size_t size)       noexcept; //!< target <- source <- 0
        static void        swap(void *target, void *source, size_t size)             noexcept; //!< memswap(target,source,size)
        static unit_t      diff(const void *a, const void *b)                        noexcept; //!< b-a
        static bool        is0(const void *addr, const size_t size)                  noexcept; //!< check only zero
        static void       *shift(void *addr, const ptrdiff_t offset)                 noexcept; //!< addr += offset
        static const void *shift(const void *addr, const ptrdiff_t offset)           noexcept; //!< addr += offset, const

        //! copy POD types
        template <typename TARGET, typename SOURCE> static inline
        void copy(TARGET &target, SOURCE &source) noexcept
        {
            assert(sizeof(TARGET)>=sizeof(SOURCE));
            (void) copy( &target, &source, sizeof(TARGET) );
        }

        //! helper with type casting
        template <typename T> static inline
        T *naught(T *obj) noexcept { assert(NULL!=obj); return static_cast<T*>(zset(obj,sizeof(T))); }

        //! helper to shift addresses
        template <typename T> static inline T *haul(T *obj, const ptrdiff_t n) noexcept
        {
            static const ptrdiff_t sz = sizeof(T);
            return static_cast<T*>(shift(obj,sz*n));
        }

        //! next object
        template <typename T> static inline T *next(T *obj) noexcept
        {
            static const ptrdiff_t sz = sizeof(T);
            return static_cast<T*>(shift(obj,sz));

        }

        //! previous object
        template <typename T> static inline T *prev(T *obj) noexcept
        {
            static const ptrdiff_t sz = -ptrdiff_t(sizeof(T));
            return static_cast<T*>(shift(obj,sz));
        }

        //! reference to inner type
        template <typename TARGET, typename SOURCE> static inline
        TARGET & access(SOURCE &source, const ptrdiff_t offset) noexcept
        {
            return *static_cast<TARGET *>( shift(&source,offset) );
        }

    };

    //! static field zset
#define YACK_STATIC_ZSET(FIELD) yack::out_of_reach::zset(FIELD,sizeof(FIELD))

    //! out of reach type coercion
    template <typename T, typename U> inline
    T *coerce_cast(U *field) noexcept { return static_cast<T*>(out_of_reach::address(field) ); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    const T *coerce_cast(const U *field) noexcept { return static_cast<const T*>(out_of_reach::address(field) ); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    T &coerce_to(U &args) noexcept { return *coerce_cast<T,U>(&args); }
    
    //! out of reach type coercion
    template <typename T, typename U> inline
    const T &coerce_to(const U &args) noexcept { return *coerce_cast<T,U>(&args); }
    
}

#endif
