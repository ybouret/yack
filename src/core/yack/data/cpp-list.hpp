

//! \file

#ifndef YACK_DATA_CXX_LIST_INCLUDED
#define YACK_DATA_CXX_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    
#define YACK_LIST_CHECK(NODE) \
assert(NULL!=NODE); assert(NULL==(NODE)->next); assert(NULL==(NODE)->prev)
    
    template <typename NODE>
    class cxx_list_of : public list_of<NODE>
    {
    public:
        inline virtual ~cxx_list_of() throw() {}
        inline explicit cxx_list_of() throw() : list_of<NODE> () {}
        inline virtual void release() throw()
        
    private:
        YACK_DISABLE_ASSIGN(cxx_list_of);
    };
    
}

#endif
