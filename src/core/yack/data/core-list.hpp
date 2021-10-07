//! \file

#ifndef YACK_DATA_CORE_LIST_INCLUDED
#define YACK_DATA_CORE_LIST_INCLUDED 1


namespace yack
{
    
    template <typename NODE> struct core_list_of
    {
        NODE  *head;
        NODE  *tail;
        size_t size;
    };

}

#endif
