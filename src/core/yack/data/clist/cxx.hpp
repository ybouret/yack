
//! \file

#ifndef YACK_DATA_CXX_CLIST_INCLUDED
#define YACK_DATA_CXX_CLIST_INCLUDED 1

#include "yack/data/clist.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    template <typename NODE>
    class cxx_clist_of : public clist_of<NODE>, public releasable
    {
    public:

        explicit cxx_clist_of() noexcept : clist_of<NODE>(), releasable() {}
        virtual ~cxx_clist_of() noexcept {}

        virtual void release() noexcept
        {

        }

    private:
        YACK_DISABLE_ASSIGN(cxx_clist_of);
    };

}

#endif

