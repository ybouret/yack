
//! \file

#ifndef YACK_COUNTED_INCLUDED
#define YACK_COUNTED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! base class for counted object
    //
    //__________________________________________________________________________
    class counted
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~counted() throw();
        explicit counted() throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void     withhold() throw();         //!< increase quantity
        bool     liberate() throw();         //!< decrease quantity, true if 0==quantity()
        size_t   quantity() const throw();   //!< current  quantity

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted);
        size_t nref;
    };
    
}

#endif

