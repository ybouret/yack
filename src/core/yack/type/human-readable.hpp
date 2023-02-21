//! \file

#ifndef YACK_HUMAN_READABLE_INCLUDED
#define YACK_HUMAN_READABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! converting (big) integers to human readable form
    //
    //__________________________________________________________________________
    class human_readable
    {
    public:
        
        const unsigned ip; //!< integer par in [0..1023]
        const unsigned fp; //!< fractional part
        const char     rx; //!< radix : b,k,M,G,T,P,E

        human_readable(const uint64_t)               noexcept; //!< build
        human_readable(const human_readable &)       noexcept; //!< copy
        ~human_readable()                            noexcept; //!< clear
        human_readable & operator=(human_readable &) noexcept; //!< assign

    };

}


#endif

