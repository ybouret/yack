//! \file

#ifndef YACK_READABLE_INCLUDED
#define YACK_READABLE_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! readable class
    //
    //__________________________________________________________________________
    template <typename T>
    class readable
    {
    public:
        YACK_DECL_ARGS(T,type);
        
        inline virtual ~readable() throw() {}

        virtual size_t      size()                   const throw() = 0; //!< handled items
        virtual const_type &operator[](const size_t) const throw() = 0; //!< in [1..size]

    protected:
        inline explicit readable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(readable);
    };

}

#endif
