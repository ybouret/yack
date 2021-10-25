//! \file

#ifndef YACK_CONTAINER_INCLUDED
#define YACK_CONTAINER_INCLUDED 1

#include "yack/container/collection.hpp"

namespace yack
{
  
    class container : public collection
    {
    public:
        virtual ~container() throw();
        
    protected:
        explicit container() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(container);
        
    };
    
}


#endif

