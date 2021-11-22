//! \file

#ifndef YACK_ASSOCIATIVE_INCLUDED
#define YACK_ASSOCIATIVE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/container.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! associative class interface
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class associative : public container
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);        //!< aliases
        YACK_DECL_ARGS(KEY,key_type);  //!< aliases

        //______________________________________________________________________
        //
        //! search interface
        //______________________________________________________________________
        virtual const_type *search(param_key_type) const throw() = 0;

        //______________________________________________________________________
        //
        //! remove interface
        //______________________________________________________________________
        virtual bool        remove(param_key_type)       throw() = 0;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~associative() throw() {} //!< cleanup

    protected:
        inline explicit associative() throw() {} //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(associative);
    };

}

#endif

