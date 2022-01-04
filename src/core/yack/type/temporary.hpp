
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
        inline explicit temporary(T &host, T  temp) : who(host), old(host)
        { host = temp; }

        //! restore original host value
        inline virtual ~temporary() throw() { who = old; }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(temporary);
        T  &who;
        T   old;
    };

    //__________________________________________________________________________
    //
    //
    //! temporary increase a value
    //
    //__________________________________________________________________________
    template <typename T, const size_t N=1>
    class temporary_increase : public temporary<T>
    {
    public:
        inline explicit temporary_increase(T &host) : temporary<T>(host,host+N) {} //!< save and set
        inline virtual ~temporary_increase() throw() {}                            //!< reset

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(temporary_increase);
    };
}

#endif
