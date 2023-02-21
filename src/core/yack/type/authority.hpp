//! \file

#ifndef YACK_TYPE_AUTHORITY_INCLUDED
#define YACK_TYPE_AUTHORITY_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! authority from a foreign class
    //
    //__________________________________________________________________________
    template <typename T> class authority
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,host); //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~authority()        noexcept {}                          //!< cleanup
        inline explicit authority(T &user) noexcept : host_( coerce(user) ) {}  //!< setup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline host       & operator*()  noexcept       { return host_; } //!< access
        inline const_host & operator*()  const noexcept { return host_; } //!< access
        
        inline host       * operator->()  noexcept       { return &host_; } //!< access
        inline const_host * operator->()  const noexcept { return &host_; } //!< access
        
    private:
        mutable_host &host_;
        YACK_DISABLE_ASSIGN(authority);
    };
}

#endif

