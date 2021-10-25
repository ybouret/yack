


//! \file

#ifndef YACK_CONTIGUOUS_INCLUDED
#define YACK_CONTIGUOUS_INCLUDED 1

#include "yack/container/writable.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! contiguous writable T
    //
    //__________________________________________________________________________
    template <typename T>
    class contiguous : public writable<T>
    {
    public:
        inline virtual ~contiguous() throw() {} //!< cleanup

    protected:
        inline explicit contiguous() throw() {} //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contiguous);
    };

}

#endif

