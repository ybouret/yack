
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
        negative = -1, //!< value <  0
        __zero__ =  0, //!< value == 0
        positive =  1 //!< 0     < value
    };
    
    enum sign_pair
    {
        nn_pair,
        nz_pair,
        np_pair,
        
        zn_pair,
        zz_pair,
        zp_pair,
        
        pn_pair,
        pz_pair,
        pp_pair
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
        
        static sign_type opposite(const sign_type) throw();
        static void      change(sign_type&) throw();
        static sign_pair pair(const sign_type lhs, const sign_type rhs) throw();
        
        
    };
    
   
    
    
}

#endif

