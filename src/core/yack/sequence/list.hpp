

//! \file

#ifndef YACK_LIST_INCLUDED
#define YACK_LIST_INCLUDED 1

#include "yack/container/sequence.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{
    
    template <typename T>
    class list : public sequence<T>
    {
    public:
        inline virtual ~list() throw() {}
        
    protected:
        inline explicit list() throw() : container() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(list);
    };
    
}


#endif

