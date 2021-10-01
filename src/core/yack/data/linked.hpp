//! \file

#ifndef YACK_DATA_LINKED_INCLUDED
#define YACK_DATA_LINKED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    

    class linked
    {
    public:
        virtual ~linked() throw();

        const size_t size;
        
    protected:
        explicit linked()   throw();
        void     increase() throw();
        void     decrease() throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(linked);
    };

}

#endif

