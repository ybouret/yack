//! \file

#ifndef YACK_TYPE_AUTHORITY_INCLUDED
#define YACK_TYPE_AUTHORITY_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    template <typename T> class authority
    {
    public:
        YACK_DECL_ARGS_(T,host); //!< aliases

        inline virtual ~authority() throw() {}
        inline explicit authority(T &user) throw() : host_( coerce(user) ) {}

        inline host       & operator->() throw()       { return host_; }
        inline const_host & operator->() const throw() { return host_; } //!< drill-down

    private:
        mutable_host &host_;
        YACK_DISABLE_ASSIGN(authority);
    };
}

#endif

