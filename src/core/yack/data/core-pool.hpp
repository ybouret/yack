//! \file

#ifndef YACK_DATA_CORE_POOL_INCLUDED
#define YACK_DATA_CORE_POOL_INCLUDED 1


namespace yack
{

    template <typename NODE> struct core_pool_of
    {
        NODE  *head;
        size_t size;
    };

#define YACK_CORE_POOL_INIT 0,0

}

#endif
