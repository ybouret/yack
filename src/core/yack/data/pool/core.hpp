//! \file

#ifndef YACK_DATA_CORE_POOL_INCLUDED
#define YACK_DATA_CORE_POOL_INCLUDED 1

#include <cassert>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! legacy pool for compact handling
    //
    //__________________________________________________________________________
    template <typename NODE> struct core_pool_of
    {
        NODE  *head; //!< top/head elememt
        size_t size; //!< number of elements

        //! inline pop
        inline NODE *pop() noexcept {
            assert(head); assert(size>0);
            NODE *node=head;
            head=head->next;
            node->next=NULL;
            --size;
            return node;
        }

        //! inline push
        inline NODE *push(NODE *node) noexcept {
            assert(node); assert(NULL==node->next);
            node->next = head;
            head       = node;
            ++size;
            return node;
        }
        
        
    };

    //__________________________________________________________________________
    //
    //
    //! core pool initializer
    //
    //__________________________________________________________________________
#define YACK_CORE_POOL_INIT 0,0

}

#endif
