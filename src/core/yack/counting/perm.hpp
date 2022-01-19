
//! \file

#ifndef YACK_COUNTING_PERMUTATION_INCLUDED
#define YACK_COUNTING_PERMUTATION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! handling permutations
    //
    //__________________________________________________________________________
    class permutation : public schedule
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit permutation(const size_t n); //!< setup
        virtual ~permutation() throw();       //!< cleanup
        
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t size() const throw(); //!< n

        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(permutation);
        virtual void   on_boot() throw();       //!< [1..n]
        virtual bool   on_next() throw();       //!< find next permutation
    };
    
}

#endif
