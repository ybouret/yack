//! \file

#ifndef YACK_RTTI_INCLUDED
#define YACK_RTTI_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"
#include <typeinfo>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! Run Time Type Information system
    //
    //__________________________________________________________________________
    class rtti
    {
    public:
        virtual ~rtti() throw(); //!<cleanup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(rtti);
    };

}

#endif

