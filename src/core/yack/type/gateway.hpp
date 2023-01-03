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
        YACK_DECL_ARGS_(T,interface); //!< aliases

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        inline interface       & operator*()        throw() { return (interface&) bulk(); } //!< access
        inline const_interface & operator*()  const throw() { return              bulk(); } //!< access
        inline interface       * operator->()       throw() { return (interface*)&bulk(); } //!< access
        inline const_interface * operator->() const throw() { return             &bulk(); } //!< access

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~gateway() throw() {}                         //!< cleanup

    protected:
        inline explicit gateway() throw() {} //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_interface &bulk() const throw() = 0;
    };

}

#endif
