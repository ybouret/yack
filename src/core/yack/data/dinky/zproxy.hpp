//! \file

#ifndef YACK_DINKY_ZPROXY_INCLUDED
#define YACK_DINKY_ZPROXY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! proxy to convert class into pointer like
    //
    //__________________________________________________________________________
    template <typename HOST>
    class zproxy
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zproxy() noexcept : host()      {} //!< setup
        inline virtual ~zproxy() noexcept               {} //!< cleanup
        inline zproxy(const zproxy &) noexcept : host() {} //!< copy is do nothing...
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline HOST *       operator->()       noexcept { return &host; } //!< access
        inline const HOST & operator->() const noexcept { return &host; } //!< access
        
    private:
        HOST host;
        YACK_DISABLE_ASSIGN(zproxy);
    };
}

#endif

