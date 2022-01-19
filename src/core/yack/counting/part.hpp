

//! \file

#ifndef YACK_COUNTING_PARTITION_INCLUDED
#define YACK_COUNTING_PARTITION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! handling partitions of integers
    //
    //__________________________________________________________________________
    class partition : public schedule
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit partition(const size_t n); //!< setup n>0
        virtual ~partition() throw();       //!< cleanup

        static apn compute(const size_t n);

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t size() const throw(); //!< current size in 1..n
        virtual void   boot() throw();       //!< size()=1, [1] = n
        virtual bool   next() throw();       //!< up to [1 1...1 1]
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(partition);
    };
    
}

#endif
