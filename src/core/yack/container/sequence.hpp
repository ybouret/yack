
//! \file

#ifndef YACK_SEQUENCE_INCLUDED
#define YACK_SEQUENCE_INCLUDED 1

#include "yack/container.hpp"

namespace yack
{
    
    template <typename T>
    class sequence : public container
    {
    public:
        inline virtual ~sequence() throw() {}
        
    protected:
        inline explicit sequence() throw() : container() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sequence);
    };
    
}


#endif

