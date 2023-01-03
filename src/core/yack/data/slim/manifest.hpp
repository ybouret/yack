
//! \file

#ifndef YACK_DATA_SLIM_MANIFEST_INCLUDED
#define YACK_DATA_SLIM_MANIFEST_INCLUDED 1

#include "yack/data/slim/zpool.hpp"

namespace yack
{


    template <typename NODE, typename ZPOOL>
    class slim_manifest
    {
    public:
        typedef cxx_list_of<NODE> list_type;
        typedef slim_zpool<NODE>  pool_type;


        const list_type  listing;
        const ZPOOL      deposit;

        inline virtual ~slim_manifest() throw() {}
        inline explicit slim_manifest() throw() : listing(), deposit() {}
        inline explicit slim_manifest(const ZPOOL &shared) throw() : listing(), deposit(shared) {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_manifest);

    };

}

#endif
