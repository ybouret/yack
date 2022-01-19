//! \file

#ifndef YACK_COUNTING_INCLUDED
#define YACK_COUNTING_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! unsigned integer for cardinality
    //
    //__________________________________________________________________________
    typedef uint64_t cardinality_t;

    //__________________________________________________________________________
    //
    //
    //! counting interface
    //
    //__________________________________________________________________________
    class counting
    {
    public:
        virtual     ~counting() throw();     //!< cleanup
        virtual void boot()     throw() = 0; //!< boot state
        virtual bool next()     throw() = 0; //!< next state

        const cardinality_t index;
        const cardinality_t total;


    protected:
        explicit counting(const cardinality_t num) throw(); //!< setup with num>0
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counting);
    };
    
}

#endif

