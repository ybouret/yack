
//! \file

#ifndef YACK_ENDIAN_INCLUDED
#define YACK_ENDIAN_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! testing endianness
    //
    //__________________________________________________________________________
    struct endian
    {
        static bool BE() noexcept; //!< if Big    Endian
        static bool LE() noexcept; //!< of Little Endian

        typedef void      (*swap_proc)(void *addr, const size_t); //!< swap funciton
        static  swap_proc   BEswap()              noexcept;        //!< get swap/nope function
        static  void       *BEaddr(void *,size_t) noexcept;        //!< BE at address

        //! integral type BEswap
        template <typename T> static inline
        T swap_be(const T &source) noexcept {
            static const swap_proc swp = BEswap();
            T target = source;
            swp(&target,sizeof(target));
            return target;
        }

    };
}

#endif

