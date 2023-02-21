
//! \file

#ifndef YACK_PTR__INCLUDED
#define YACK_PTR__INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! common data for smart pointer
    //
    //__________________________________________________________________________
    class ptr_
    {
    public:
        static const char nil[]; //!< "(nil)"
        virtual ~ptr_() noexcept; //!< cleanup

    protected:
        explicit ptr_() noexcept; //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ptr_);
    };

}

#endif
