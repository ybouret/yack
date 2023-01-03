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

        inline inward       & operator*()        throw() { return (inward&) bulk(); } //!< access
        inline const_inward & operator*()  const throw() { return           bulk(); } //!< access
        inline inward       * operator->()       throw() { return (inward*)&bulk(); } //!< access
        inline const_inward * operator->() const throw() { return          &bulk(); } //!< access

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~gateway() throw() {}                         //!< cleanup

    protected:
        inline explicit gateway() throw() {} //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_inward &bulk() const throw() = 0;
    };

}

#endif
