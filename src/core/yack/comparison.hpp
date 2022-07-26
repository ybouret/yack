//! \file

#ifndef YACK_COMPARISON_INCLUDED
#define YACK_COMPARISON_INCLUDED 1

#include "yack/type/utils.hpp"
#include "yack/type/abs.hpp"

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
        //! increasing
        //______________________________________________________________________
        template <typename T> static inline
        int increasing(const T &lhs, const T &rhs) throw()
        {
            return (lhs<rhs) ? -1 : (rhs<lhs ? 1 : 0);
        }

        //______________________________________________________________________
        //
        //! increasing absolute value
        //______________________________________________________________________
        template <typename T> static inline
        int increasing_abs(const T &lhs, const T &rhs) throw()
        {
            const T L = absolute(lhs);
            const T R = absolute(rhs);
            return (L<R) ? -1 : (R<L ? 1 : 0);
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
        //! increasing absolute value
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing_abs(const T &lhs, const T &rhs) throw()
        {
            const T L = absolute(lhs);
            const T R = absolute(rhs);
            return (L<R) ? 1 : (R<L ? -1 : 0);
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
                if( compare(*curr,*iter) > 0 ) return false;
            }
            return true;
        }

        //______________________________________________________________________
        //
        //! test ordered sequence
        //______________________________________________________________________
        template <typename SEQUENCE, typename FUNC> static inline
        bool ordered(SEQUENCE &seq, FUNC &compare)
        {
            return ordered(seq.begin(),seq.size(),compare);
        }


        //______________________________________________________________________
        //
        //! lexicographic comparison of same lengths arrays[1..n]
        //______________________________________________________________________
        template <typename T> static inline
        int lexicographic(const T *lhs, const T *rhs, const size_t n) throw()
        {
            assert(NULL!=lhs);
            assert(NULL!=rhs);
            for(size_t i=1;i<=n;++i)
            {
                const T &L = *(++lhs);
                const T &R = *(++rhs);
                if(L<R)
                    return -1;
                else
                {
                    if(R<L)
                        return 1;
                    else
                        continue;
                }
            }
            return 0;
        }
        
    };
    
}

#endif

