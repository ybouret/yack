
//! \file

#ifndef YACK_DATA_RAW_LIST_INCLUDED
#define YACK_DATA_RAW_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/object.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list with zombie NODE 
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zlist_of : public list_of<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zlist_of() throw() : list_of<NODE>()  {}
        inline virtual ~zlist_of() throw() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zlist_of);
    };

}

#endif
