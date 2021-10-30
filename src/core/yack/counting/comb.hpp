

//! \file

#ifndef YACK_COUNTING_COMBINATION_INCLUDED
#define YACK_COUNTING_COMBINATION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! handling combinations
    //
    //__________________________________________________________________________
    class combination : public schedule
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit combination(const size_t n, const size_t k); //!< setup
        virtual ~combination() throw();                       //!< setup
        
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t size() const throw(); //!< k
        virtual void   boot() throw();       //!< [1..k]
        virtual bool   next() throw();       //!< find next combination
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(combination);
    };
    
}

#endif
