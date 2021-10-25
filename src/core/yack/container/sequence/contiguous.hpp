


//! \file

#ifndef YACK_CONTIGUOUS_INCLUDED
#define YACK_CONTIGUOUS_INCLUDED 1

#include "yack/container/writable.hpp"

namespace yack
{

    template <typename T>
    class contiguous : public writable<T>
    {
    public:
        inline virtual ~contiguous() throw() {}

    protected:
        inline explicit contiguous() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contiguous);
    };

}

#endif

