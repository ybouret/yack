
//! \file

#ifndef YACK_TYPE_TEMPORARY_INCLUDED
#define YACK_TYPE_TEMPORARY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! set/reset temporary value
    //
    //__________________________________________________________________________
    template <typename T>
    class temporary
    {
    public:

        //! backup host value, then host=temp
        inline   temporary(T &host, T  temp) : who(host), old(host)
        { host = temp; }

        //! restore original host value
        inline ~temporary() throw() { who = old; }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(temporary);
        T  &who;
        T   old;
    };

}

#endif
