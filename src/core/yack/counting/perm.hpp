
//! \file

#ifndef YACK_COUNTING_PERMUTATION_INCLUDED
#define YACK_COUNTING_PERMUTATION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{
    
    class permutation : public schedule
    {
    public:
        explicit permutation(const size_t);
        virtual ~permutation() throw();
        
        virtual size_t size() const throw();
        virtual void   boot() throw();
        virtual bool   next() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(permutation);
    };
    
}

#endif
