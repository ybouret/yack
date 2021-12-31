//! \file

#ifndef YACK_TYPE_GATEWAY_INCLUDED
#define YACK_TYPE_GATEWAY_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{

    template <typename T> class gateway
    {
    public:
        YACK_DECL_ARGS_(T,type);

        inline virtual ~gateway() throw() {}

        inline type       & operator*()       throw() { return (type&)bulk(); }
        inline const_type & operator*() const throw() { return bulk();        }

    protected:
        inline explicit gateway() throw() {}
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_type &bulk() const throw() = 0;
    };

}

#endif
