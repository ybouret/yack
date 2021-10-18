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
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t      size()                   const throw() = 0; //!< handled items
        virtual const_type &operator[](const size_t) const throw() = 0; //!< in [1..size]

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~readable() throw() {}
    protected:
        inline explicit readable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(readable);
    };

}

#endif
