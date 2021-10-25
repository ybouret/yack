

//! \file

#ifndef YACK_THIN_ARRAY_INCLUDED
#define YACK_THIN_ARRAY_INCLUDED 1

#include "yack/container/sequence/contiguous.hpp"

namespace yack
{

    template <typename T>
    class thin_array : public contiguous<T>
    {
    public:

        inline virtual ~thin_array() throw() {}
        inline explicit thin_array();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(thin_array);
    };

}

#endif

