
//! \file

#ifndef YACK_SYNC_QUARK_CONDITION_INCLUDED
#define YACK_SYNC_QUARK_CONDITION_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            
            class  condition;
            struct condition_api
            {
                static condition *init();                      //!< create a condition
                static void       quit(condition * &) throw(); //!< delete a condition
            };
        }
    }
}

#endif

