//! \file

#ifndef YACK_TYPE_GATEWAY_INCLUDED
#define YACK_TYPE_GATEWAY_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! gateway to internal type
    //
    //__________________________________________________________________________
    template <typename T> class gateway
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,inward); //!< aliases

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        inline inward       & operator*()        noexcept { return (inward&) bulk(); } //!< access
        inline const_inward & operator*()  const noexcept { return           bulk(); } //!< access
        inline inward       * operator->()       noexcept { return (inward*)&bulk(); } //!< access
        inline const_inward * operator->() const noexcept { return          &bulk(); } //!< access

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~gateway() noexcept {}                         //!< cleanup

    protected:
        inline explicit gateway() noexcept {} //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_inward &bulk() const noexcept = 0;
    };

}

#endif
