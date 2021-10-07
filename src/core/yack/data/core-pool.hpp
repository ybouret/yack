//! \file

#ifndef YACK_DATA_CORE_POOL_INCLUDED
#define YACK_DATA_CORE_LIST_INCLUDED 1


namespace yack
{

    template <typename NODE> struct core_pool_of
    {
        NODE  *head;
        size_t size;
    };

}

#endif
