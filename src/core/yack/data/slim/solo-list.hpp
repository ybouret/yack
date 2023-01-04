
//! \file

#ifndef YACK_DATA_SLIM_SOLO_LIST_INCLUDED
#define YACK_DATA_SLIM_SOLO_LIST_INCLUDED 1

#include "yack/data/slim/solo.hpp"
#include "yack/data/slim/registry.hpp"

namespace yack
{

    template <typename T>
    class solo_list : public slim_solo<T, slim_node<T>, slim_registry >
    {
    public:
        typedef slim_node<T>                         node_type;
        typedef slim_solo<T,node_type,slim_registry> body_type;

        inline explicit solo_list() throw()  : body_type()  {}
        inline virtual ~solo_list() throw()                 {}
        inline solo_list(const solo_list &_) : body_type(_) {}

    private:
        YACK_DISABLE_ASSIGN(solo_list);
    };

}

#endif
