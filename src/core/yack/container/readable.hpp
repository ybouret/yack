

//! \file

#ifndef YACK_READABLE_INCLUDED
#define YACK_READABLE_INCLUDED 1

#include "yack/setup.hpp"

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
        inline virtual ~readable() throw() {}

        virtual size_t size() const throw() = 0; //!< handled items

    protected:
        inline explicit readable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(readable);
    };

}

#endif
