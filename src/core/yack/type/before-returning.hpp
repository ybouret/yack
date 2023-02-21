//! \file

#ifndef YACK_TYPE_BEFORE_RETURNING_INCLUDED
#define YACK_TYPE_BEFORE_RETURNING_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! always execute code before returning
    //
    //__________________________________________________________________________
    template <typename T>
    class before_returning
    {
    public:
        //! method prototype
        typedef void (T::*cleanup)();

        //! setup
        inline before_returning(T         &Object,
                                cleanup    Method,
                                const bool Ensure) :
        host(Object),
        code(Method)
        {
            assert(0!=code);
            if(Ensure) call();
        }

        //! cleanup
        inline ~before_returning() noexcept {
            try
            {
                call();
            }
            catch(...)
            {
            }
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(before_returning);
        T &     host;
        cleanup code;
        inline void call() noexcept
        {
            (host.*code)();
        }
    };

}

#endif
