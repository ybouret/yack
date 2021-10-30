

//! \file

#ifndef YACK_COUNTING_COMBINATION_INCLUDED
#define YACK_COUNTING_COMBINATION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{
    
    class combination : public schedule
    {
    public:
        explicit combination(const size_t n, const size_t k);
        virtual ~combination() throw();
        
        virtual size_t size() const throw();
        virtual void   boot() throw();
        virtual bool   next() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(combination);
    };
    
}

#endif
