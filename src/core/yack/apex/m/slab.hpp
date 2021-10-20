//! \file

#ifndef YACK_APEX_SLAB_INCLUDED
#define YACK_APEX_SLAB_INCLUDED 1


#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/data/core-pool.hpp"

namespace yack
{



    namespace apex
    {
        struct slab
        {
            slab   *next;
            slab   *prev;
            size_t  exp2;
            size_t  size;
            typedef core_pool_of<slab> repo;
        };
        





    }

}

#endif

