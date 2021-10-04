
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
            
            struct mutex;
            class  condition;
            
            condition *condition_create();                    //!< create a condition
            void       condition_delete(condition *) throw(); //!< delete a condition
        }
    }
}

#endif

