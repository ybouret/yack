
//! \file

#ifndef YACK_SYNC_QUARK_CONDITION_INCLUDED
#define YACK_SYNC_QUARK_CONDITION_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            //______________________________________________________________________
            //
            //
            // forward declaration
            //
            //______________________________________________________________________
            class  condition;

            //__________________________________________________________________
            //
            //
            //! condition API
            /**
             the memory of the condition is within a compact memory::arena
             of the internal atelier.
             */
            //
            //__________________________________________________________________
            struct condition_api
            {
                static condition *init();                      //!< create a condition
                static void       quit(condition * &) noexcept; //!< delete a condition
            };
        }
    }
}

#endif

