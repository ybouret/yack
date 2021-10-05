
//! \file

#ifndef YACK_SYSTEM_AT_EXIT_INCLUDED
#define YACK_SYSTEM_AT_EXIT_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! handling at_exit procedure with UNIQUE longevity
    //
    //__________________________________________________________________________
    struct at_exit
    {
        static const size_t    capacity = 64;     //!< maximum number of procedures
        typedef void         (*callback)(void *); //!< prototype
        typedef int16_t        longevity;         //!< alias
        static const longevity uttermost = integral_for<longevity>::maximum; //!< longest living
       
        //! call proc(args) depending on longevity
        static void  perform(callback,void*,const longevity);
        
        callback  proc; //!< procedure
        void     *args; //!< arguments
        longevity rank; //!< ranked by longevity
    };
}

#endif

