
//! \file

#ifndef YACK_COUNTED_INCLUDED
#define YACK_COUNTED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! base class for counted object
    //
    //__________________________________________________________________________
    class counted
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~counted() noexcept;
        explicit counted() noexcept;

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void     withhold() noexcept;         //!< increase quantity
        bool     liberate() noexcept;         //!< decrease quantity, true if 0==quantity()
        size_t   quantity() const noexcept;   //!< current  quantity

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted);
        size_t nref;
    };
    
}

#endif

