
//! \file

#ifndef YACK_DATA_RAW_LIST_INCLUDED
#define YACK_DATA_RAW_LIST_INCLUDED 1

#include "yack/data/list.hpp"

namespace yack
{
    
#define YACK_LIST_CHECK(NODE) \
assert(NULL!=NODE); assert(NULL==(NODE)->next); assert(NULL==(NODE)->prev)
    
    template <typename NODE>
    class raw_list_of : public list_of<NODE>
    {
    public:
        inline virtual ~raw_list_of() throw() {}
        inline explicit raw_list_of() throw() : list_of<NODE> () {}
        inline void     reset() throw() { this->head=NULL; this->tail=NULL; coerce(this->size) = 0; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(raw_list_of);
    };
    
}

#endif
