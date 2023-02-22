
//! \file

#ifndef YACK_CAMEO_DEF_INCLUDED
#define YACK_CAMEO_DEF_INCLUDED 1

#include "yack/memory/allocator/dyadic.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace cameo
    {

        typedef  memory::dyadic allocator_type; //!< memory allocator for add
        typedef  large_object   object_type;    //!< object type
    }

}

#endif

