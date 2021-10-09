//! \file

#ifndef YACK_DATA_CORE_LIST_INCLUDED
#define YACK_DATA_CORE_LIST_INCLUDED 1


namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! legacy list for compact handling
    //
    //__________________________________________________________________________
    template <typename NODE> struct core_list_of
    {
        NODE  *head; //!< head node
        NODE  *tail; //!< tail node
        size_t size; //!< number of nodes
                      
        inline void zero() throw() { head=0; tail=0; size=0; }
    };

    //__________________________________________________________________________
    //
    //
    //! core list initializer
    //
    //__________________________________________________________________________
#define YACK_CORE_LIST_INIT 0,0,0

}

#endif
