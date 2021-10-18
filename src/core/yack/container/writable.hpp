
//! \file

#ifndef YACK_WRITABLE_INCLUDED
#define YACK_WRITABLE_INCLUDED 1

#include "yack/container/readable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! writable class
    //
    //__________________________________________________________________________
    template <typename T>
    class writable : public readable<T>
    {
    public:
        typedef typename readable<T>::type       type;
        typedef typename readable<T>::const_type type;

        inline virtual ~writable() throw() {}

        virtual type &operator[](const size_t) throw() = 0; //!< in [1..size]

    protected:
        inline explicit writable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(writable);
    };

}

#endif
