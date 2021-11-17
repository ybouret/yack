//! \file

#ifndef YACK_ASSOCIATIVE_INCLUDED
#define YACK_ASSOCIATIVE_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{

    template <typename KEY, typename T>
    class associative
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        inline virtual ~associative() throw() {}

        virtual const_type *search(param_key_type) const throw() = 0;

    protected:
        inline explicit associative() throw() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(associative);
    };

}

#endif

