
//! \file

#ifndef YACK_TYPE_DESTRUCT_INCLUDED
#define YACK_TYPE_DESTRUCT_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! call destructor
    //
    //__________________________________________________________________________
    template <typename T> inline
    void destruct(T *item) throw()
    {
        assert(item);
        item->~T();
    }

    //__________________________________________________________________________
    //
    //
    //! call destructor and return memory address
    //
    //__________________________________________________________________________
    template <typename T> inline
    T *destructed(T *item) throw()
    {
        destruct(item);
        return item;
    }


}

#endif
