
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
    struct endianness
    {
        static bool BE() throw(); //!< if Big    Endian
        static bool LE() throw(); //!< of Little Endian

        typedef void      (*swap_proc)(void *addr, const size_t); //!< swap funciton
        static  swap_proc   BEswap() throw();                     //!< get swap/nope function
        static  void       *BEaddr(void *,size_t) throw();        //!< BE at address
    };
}

#endif

