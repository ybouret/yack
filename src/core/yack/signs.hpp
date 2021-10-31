
//! \file
#ifndef YACK_SIGNS_INCLUDED
#define YACK_SIGNS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! system wide named type
    //__________________________________________________________________________
    enum sign_type
    {
        negative, //!< value <  0
        __zero__, //!< value == 0
        positive  //!< 0     < value
    };
    
    //__________________________________________________________________________
    //
    //! helpers for signs
    //__________________________________________________________________________
    struct __sign
    {
        //! named sign
        static const char *text(const sign_type) throw();
        
        //! value to sign
        template <typename T> static inline
        sign_type of(const T value) throw()
        {
            return (value<0?negative:(0<value?positive:__zero__));
        }
        
    };
    
}

#endif

