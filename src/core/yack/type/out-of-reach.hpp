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
        static void        zset(void *addr, const size_t size)                       throw(); //!< memset(addr,0,size)
        static void        copy(void *target, const void *source, const size_t size) throw(); //!< memcpy(target,source,size)
        static void        move(void *target, const void *source, const size_t size) throw(); //!< memmove(target,source,size)
        static unit_t      diff(const void *a, const void *b) throw();                        //!< b-a
    };

}

#endif
