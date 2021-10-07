//! \file

#ifndef YACK_DATA_CORE_POOL_INCLUDED
#define YACK_DATA_CORE_POOL_INCLUDED 1


namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! legacy pool for compact handling
    //
    //__________________________________________________________________________
    template <typename NODE> struct core_pool_of
    {
        NODE  *head; //!< top/head elememt
        size_t size; //!< number of elements
    };

    //__________________________________________________________________________
    //
    //
    //! core pool initializer
    //
    //__________________________________________________________________________
#define YACK_CORE_POOL_INIT 0,0

}

#endif
