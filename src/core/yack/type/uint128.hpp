//! \file

#ifndef YACK_UINT128_INCLUDED
#define YACK_UINT128_INCLUDED 1

#include "yack/system/setup.h"
#include <iosfwd>

namespace yack
{

    //__________________________________________________________________________
    //
    //! 128 bits
    //__________________________________________________________________________
    class uint128_t
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        static const size_t bits  = 128;      //!< alias
        static const size_t bytes = bits/8;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        ~uint128_t()                             throw(); //!< cleanup
        uint128_t(const uint128_t &)             throw(); //!< copy
        uint128_t & operator=(const uint128_t &) throw(); //!< assign
        uint128_t()                              throw(); //!< setup (zero)
        uint128_t(uint64_t lo)                   throw(); //!< setup 64-bits

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        uint8_t       & operator[](const size_t i)       throw(); //!< access
        const uint8_t & operator[](const size_t i) const throw(); //!< access, const

        friend bool operator==(const uint128_t &lhs, const uint128_t &rhs) throw(); //!< test equality
        friend bool operator!=(const uint128_t &lhs, const uint128_t &rhs) throw(); //!< test equality


        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        friend std::ostream & operator<<(std::ostream &, const uint128_t &); //!< display

    private:
        uint8_t h[bytes];
    };
}

#endif

