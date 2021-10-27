


//! \file

#ifndef YACK_CONTIGUOUS_INCLUDED
#define YACK_CONTIGUOUS_INCLUDED 1

#include "yack/container/writable.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! contiguous writable T
    //
    //__________________________________________________________________________
    template <typename T>
    class contiguous : public writable<T>
    {
    public:
        typedef typename writable<T>::type       type;
        typedef typename writable<T>::const_type const_type;

        inline virtual ~contiguous() throw() {} //!< cleanup

        virtual type       * operator*()       throw() = 0; //!< access to [1..size()]
        virtual const type * operator*() const throw() = 0; //!< access to [1..size()]


    protected:
        inline explicit contiguous() throw() {} //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contiguous);
    };

}

#endif

