
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
        virtual ~counted() throw();
        explicit counted() throw();
        void     withhold() throw();
        bool     liberate() throw();
        size_t   quantity() const throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted);
        size_t nref;
    };
    
}

#endif

