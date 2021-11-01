//! \file

#ifndef YACK_DATA_RAW_LIST_INCLUDED
#define YACK_DATA_RAW_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/container/restartable.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! list with NODE that needs cleaning
    //
    //__________________________________________________________________________
    template <typename NODE>
    class raw_list_of : public list_of<NODE>, public restartable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~raw_list_of() throw() {}
        inline explicit raw_list_of() throw() : list_of<NODE>(), restartable() {}
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void restart() throw() { this->hard_reset_(); }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(raw_list_of);
    };
    
}

#endif
