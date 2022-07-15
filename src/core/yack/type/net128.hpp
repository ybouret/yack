//! \file

#ifndef YACK_NET128_INCLUDED
#define YACK_NET128_INCLUDED 1

#include "yack/system/setup.h"
#include <iosfwd>

namespace yack
{

    //__________________________________________________________________________
    //
    //! 128 bits network-byte-order
    //__________________________________________________________________________
    class net128_t
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        ~net128_t()                            throw(); //!< cleanup
        net128_t(const net128_t &)             throw(); //!< copy
        net128_t & operator=(const net128_t &) throw(); //!< assign
        net128_t()                             throw(); //!< setup (zero)
        net128_t(const uint8_t b)              throw(); //!< setup (byte)

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        uint8_t       & operator[](const size_t i)       throw(); //!< access
        const uint8_t & operator[](const size_t i) const throw(); //!< access, const

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        friend std::ostream & operator<<(std::ostream &, const net128_t &); //!< display

    private:
        uint8_t h[16];
    };
}

#endif

