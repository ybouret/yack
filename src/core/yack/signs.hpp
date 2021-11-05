
//! \file
#ifndef YACK_SIGNS_INCLUDED
#define YACK_SIGNS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! system wide named sign type
    //__________________________________________________________________________
    enum sign_type
    {
        negative = -1, //!< value <  0
        __zero__ =  0, //!< value == 0
        positive =  1 //!< 0     < value
    };

    //__________________________________________________________________________
    //
    //! system wide named pair of signs
    //__________________________________________________________________________
    enum sign_pair
    {
        nn_pair, //!< negative|negative
        nz_pair, //!< negative|__zero__
        np_pair, //!< negative|positive
        
        zn_pair, //!< __zero__|negative
        zz_pair, //!< __zero__|__zero__
        zp_pair, //!< __zero__|positive
        
        pn_pair, //!< positive|negative
        pz_pair, //!< positive|__zero__
        pp_pair  //!< positive|positive
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

        //! opposite sign
        static sign_type opposite(const sign_type) throw();

        //! in place change sign
        static void      change(sign_type&) throw();

        //! build lhs|rhs
        static sign_pair pair(const sign_type lhs, const sign_type rhs) throw();

        //! compute product
        static sign_type product(const sign_type lhs, const sign_type rhs) throw();
        
    };
    
   
    
    
}

#endif

