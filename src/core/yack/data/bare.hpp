//! \file

#ifndef YACK_DATA_BARE_INCLUDED
#define YACK_DATA_BARE_INCLUDED 1


#include "yack/type/args.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    //! create a bare type (with default constructor on zeroed area)
    template <typename T>
    class bare
    {
    public:
        //! aliases
        YACK_DECL_ARGS(T,type);

        //! cleanup
        inline ~bare() throw()
        {
            out_of_reach::zset( destructed( static_cast<mutable_type*>( out_of_reach::address(wksp) )), sizeof(wksp) );
        }

        //! setup
        inline  bare() : wksp()
        {
            new ( out_of_reach::zset(wksp,sizeof(wksp)) ) mutable_type();
        }

        //! setup
        inline  bare(const param_type args) : wksp()
        {
            new ( out_of_reach::zset(wksp,sizeof(wksp)) ) mutable_type(args);
        }


        //! access
        inline const_type & operator*() const throw() { return *static_cast<const_type*>( out_of_reach::address(wksp) ); }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(bare);
        void *wksp[YACK_WORDS_FOR(type)];
    };


}

#endif

