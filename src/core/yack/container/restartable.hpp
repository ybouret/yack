
//! \file

#ifndef YACK_RESTARTABLE_INCLUDED
#define YACK_RESTARTABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    class restartable
    {
    public:
        virtual ~restartable() throw();
        virtual void restart() throw() = 0;

    protected:
        explicit restartable() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(restartable);
    };

}

#endif
