
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
        
        //! search an ORDERED list
        /**
         \param l list of NODEs
         \param x value to match
         \param p matching or previous node
         */
        template <typename NODE, typename T> static inline
        bool search(const list_of<NODE> &l,
                    const T             &x,
                    NODE               *&p)
        {
            p = 0;
            switch(l.size)
            {
                case 0: return false;
                case 1:
                    switch( __sign::of(x,**l.head) )
                    {
                        case negative:             return false;
                        case __zero__: p = l.head; return true;
                        case positive: p = l.head; return false;
                    }
                default:
                    break;
            }
            assert(l.size>=2);
            exit(0);
            return false;
        }

    };

}

#endif
