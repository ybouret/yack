//! \file

#ifndef YACK_COMPARISON_INCLUDED
#define YACK_COMPARISON_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! generic comparison function
    //
    //__________________________________________________________________________
    struct comparison
    {
        //______________________________________________________________________
        //
        //! increasiong
        //______________________________________________________________________
        template <typename T> static inline
        int increasing(const T &lhs, const T &rhs) throw()
        {
            return (lhs<rhs) ? -1 : (rhs<lhs ? 1 : 0);
        }
        
        //______________________________________________________________________
        //
        //! decreasing
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing(const T &lhs, const T &rhs) throw()
        {
            return (lhs<rhs) ? 1 : (rhs<lhs ? -1 : 0);
        }
        
        //______________________________________________________________________
        //
        //! test ordered range
        //______________________________________________________________________
        template <typename ITERATOR, typename FUNC> static inline
        bool ordered(ITERATOR iter, size_t n, FUNC &compare)
        {
            switch(n)
            {
                case 0:
                case 1:
                    return true;
                default:
                    break;
            }
            assert(n>=2);
            while(--n>0)
            {
                const ITERATOR curr = iter++;
                if( compare(*curr,*iter) >0 ) return false;
            }
            return true;
        }
        
        
    };
    
}

#endif

