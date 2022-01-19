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
        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________
        void boot() throw(); //!< index=1, on_boot
        bool next() throw(); //!< ++index, on_next

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const cardinality_t index; //!< current [1..total] index
        const cardinality_t total; //!< total number of configurations

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~counting() throw();                        //!< cleanup

    protected:
        explicit counting(const cardinality_t num) throw(); //!< setup with num>0
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counting);
        virtual void on_boot()     throw() = 0; //!< boot state
        virtual bool on_next()     throw() = 0; //!< next state
    };
    
}

#endif

