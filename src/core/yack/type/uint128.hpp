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
        ~uint128_t()                             noexcept; //!< cleanup
        uint128_t(const uint128_t &)             noexcept; //!< copy
        uint128_t & operator=(const uint128_t &) noexcept; //!< assign
        uint128_t()                              noexcept; //!< setup (zero)
        uint128_t(uint64_t lo)                   noexcept; //!< setup 64-bits
        uint128_t(const uint8_t data[])          noexcept; //!< setup 128-bits

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        uint8_t       & operator[](const size_t i)       noexcept; //!< access
        const uint8_t & operator[](const size_t i) const noexcept; //!< access, const

        friend bool operator==(const uint128_t &lhs, const uint128_t &rhs) noexcept; //!< test equality
        friend bool operator!=(const uint128_t &lhs, const uint128_t &rhs) noexcept; //!< test difference


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

