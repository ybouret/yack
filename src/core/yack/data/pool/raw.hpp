//! \file

#ifndef YACK_DATA_RAW_POOL_INCLUDED
#define YACK_DATA_RAW_POOL_INCLUDED 1

#include "yack/data/pool.hpp"
#include "yack/container/restartable.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! pool with NODE that needs cleaning
    //
    //__________________________________________________________________________
    template <typename NODE>
    class raw_pool_of : public pool_of<NODE>, public restartable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~raw_pool_of() throw() {}
        inline explicit raw_pool_of() throw() : pool_of<NODE>(), restartable() {}
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void restart() throw() { this->head=NULL; coerce(this->size) = 0; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(raw_pool_of);
    };
    
}

#endif
