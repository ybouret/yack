//! \file

#ifndef YACK_TYPE_CONSTANT_INCLUDED
#define YACK_TYPE_CONSTANT_INCLUDED 1

#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/args.hpp"
#include "yack/type/transmogrify.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! making a constant field
    //
    //__________________________________________________________________________
    template <typename T>
    class const_field
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        //! cleanup
        //______________________________________________________________________
        inline ~const_field() throw()
        { out_of_reach::zset( destructed(impl), sizeof(data) ); impl=0; }

        //______________________________________________________________________
        //
        //! default constructor on a zeroed area
        //______________________________________________________________________
        inline  const_field() : impl(0), data()
        { impl = new ( zget() ) mutable_type(); }

        //______________________________________________________________________
        //
        //! 1-arg default constructor
        //______________________________________________________________________
        inline const_field(param_type args) : impl(0), data()
        { impl = new ( zget() ) mutable_type(args); }

        //______________________________________________________________________
        //
        //! 1-arg with transmogrifier
        //______________________________________________________________________
        template <typename U>
        inline const_field(const transmogrify_t &, const U &args) :
        impl(0), data()
        {
            impl = new ( zget() ) mutable_type(args);
        }

        //______________________________________________________________________
        //
        //! access
        //______________________________________________________________________
        inline const_type &operator*() const throw() { assert(impl); return *impl; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(const_field);
        mutable_type  *impl;
        void          *data[ YACK_WORDS_FOR(T) ];
        void          *zget() throw() { return out_of_reach::zset(data,sizeof(data)); }
    };

}

#endif

