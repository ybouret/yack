//! \file

#ifndef YACK_DINKY_ZPROXY_INCLUDED
#define YACK_DINKY_ZPROXY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    template <typename HOST>
    class zproxy
    {
    public:
        explicit zproxy() throw() {}
        virtual ~zproxy() throw() {}
        
        inline HOST *       operator->()       throw() { return &host; }
        inline const HOST & operator->() const throw() { return &host; }
        
    private:
        HOST host;
        YACK_DISABLE_COPY_AND_ASSIGN(zproxy);
    };
}

#endif

