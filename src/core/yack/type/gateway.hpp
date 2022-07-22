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
        YACK_DECL_ARGS_(T,type); //!< aliases

        //__________________________________________________________________________
        //
        // methods
        //__________________________________________________________________________

        inline virtual     ~gateway()         throw() {}                         //!< cleanup
        inline type       & operator*()       throw() { return (type&)bulk(); }  //!< access
        inline const_type & operator*() const throw() { return bulk();        }  //!< access
        inline type       * operator->() throw() { return (type*) &bulk(); }
        inline const_type * operator->() const throw() { return   &bulk(); }


    protected:
        inline explicit gateway() throw() {} //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_type &bulk() const throw() = 0;
    };

}

#endif
