
//! \file

#ifndef YACK_DATA_LIST_OPS_INCLUDED
#define YACK_DATA_LIST_OPS_INCLUDED 1

#include "yack/data/list.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! operation on list
    //
    //__________________________________________________________________________
    struct list_ops
    {
        
        //______________________________________________________________________
        //
        //! test node-wise equality
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        bool are_equal(const list_of<LHS> &lhs, const list_of<RHS> &rhs)
        {
            size_t n = lhs.size;
            if(n != rhs.size)
            {
                return false;
            }
            else
            {
                const LHS *l = lhs.head;
                const RHS *r = rhs.head;
                while(n-- > 0)
                {
                    if( **l != **r ) return false;
                    l=l->next;
                    r=r->next;
                }
                return true;
            }
        }
        
        
        

    };

}

#endif
