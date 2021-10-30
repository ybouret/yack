
//! \file

#ifndef YACK_COUNTING_INCLUDED
#define YACK_COUNTING_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    class counting
    {
    public:
        virtual ~counting() throw();
        
        virtual void boot() throw() = 0;
        virtual bool next() throw() = 0;
        
    protected:
        explicit counting() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counting);
    };
    
}

#endif

