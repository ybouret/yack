//! \file

#ifndef YACK_COUNTED_INCLUDED
#define YACK_COUNTED_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/ptr/counted-interface.hpp"

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
        YACK_COUNTED_DECL(); //!< declare interface

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted);
        size_t nref;
    };
    
}

#endif

