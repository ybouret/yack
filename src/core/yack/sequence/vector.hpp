
//! \file

#ifndef YACK_VECTOR_INCLUDED
#define YACK_VECTOR_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/sequence/contiguous.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    template <typename T>
    class vector : public sequence<T>, public contiguous<T>
    {
    public:
        YACK_DECL_ARGS(T,type);

        inline explicit vector() throw() {}
        inline virtual ~vector() throw() {}

        


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vector);
    };

}

#endif
