
//! \file

#ifndef YACK_ASSOCIATIVE_REGISTRY_INCLUDED
#define YACK_ASSOCIATIVE_REGISTRY_INCLUDED 1

#include "yack/container/associative.hpp"

namespace yack
{

    template <typename KEY, typename T>
    class registry : public associative<KEY,T>
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        inline virtual ~registry() throw() {}

        virtual bool insert(param_type) = 0;

    protected:
        inline explicit registry() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(registry);
    };

}

#endif

