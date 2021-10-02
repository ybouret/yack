
//! \file

#ifndef YACK_RELEASABLE_INCLUDED
#define YACK_RELEASABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    class releasable
    {
    public:
        virtual     ~releasable() throw();
        virtual void release()    throw() = 0;

    protected:
        explicit releasable() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(releasable);
    };

}

#endif
