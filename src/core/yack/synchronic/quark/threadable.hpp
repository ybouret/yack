

//! \file

#ifndef YACK_SYNC_QUARK_THREADABLE_INCLUDED
#define YACK_SYNC_QUARK_THREADABLE_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    namespace synchronic
    {

        //______________________________________________________________________
        //
        //! threadable procedure
        //______________________________________________________________________
        typedef void (*threadable)(void *);
    }

}

#endif
