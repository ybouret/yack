//! \file

#ifndef YACK_COUNTING_OPS_INCLUDED
#define YACK_COUNTING_OPS_INCLUDED 1

#include "yack/container/writable.hpp"

namespace yack
{
    namespace nubbin
    {
        struct counting
        {
            static void initialize(writable<size_t> &state) throw();
        };
    }

}

#endif


