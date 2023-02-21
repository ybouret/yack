
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
        static const char *text(const sign_type)   noexcept; //!< named sign
        static const char *symbol(const sign_type) noexcept; //!< -/0/+

        //! value to sign
        template <typename T> static inline
        sign_type of(const T value)
        {
            return (value<0?negative:(0<value?positive:__zero__));
        }

        //! comparison to sign
        template <typename T, typename U> static inline
        sign_type of(const T &lhs, const U &rhs)
        {
            return (lhs<rhs) ? negative : ( (rhs<lhs) ? positive : __zero__ );
        }
        

        //! opposite sign
        static sign_type opposite(const sign_type) noexcept;

        //! in place change sign
        static void      change(sign_type&) noexcept;

        //! build lhs|rhs
        static sign_pair pair(const sign_type lhs, const sign_type rhs) noexcept;

        //! buils sign_of(rhs) | sign_of(lhs)
        template <typename T, typename U> static inline
        sign_pair pair_of(const T &lhs, const U &rhs)
        {
            return  pair( of(lhs), of(rhs) );
        }


        //! compute product
        static sign_type product(const sign_type lhs, const sign_type rhs) noexcept;

        //! compute product of
        template <typename T, typename U> static inline
        sign_type product_of(const T &lhs, const U &rhs)
        {
            return product( of(lhs), of(rhs) );
        }

    };
    
   
    
    
}

#endif

