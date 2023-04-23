
//! \file

#ifndef YACK_IPSO_DEF_INCLUDED
#define YACK_IPSO_DEF_INCLUDED 1

#include "yack/memory/allocator/dyadic.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace ipso
    {

        typedef  memory::dyadic memory_type; //!< memory allocator for add
        typedef  large_object   object_type; //!< object type
    }

}

#endif
