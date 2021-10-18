
//! \file

#ifndef YACK_TYPE_TEMPORARY_INCLUDED
#define YACK_TYPE_TEMPORARY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    template <typename T>
    class temporary
    {
    public:
        inline   temporary(T &host, T  temp) : who(host), old(host)
        { host = temp; }

        inline ~temporary() throw() { who = old; }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(temporary);
        T  &who;
        T   old;
    };

}

#endif
