//! \file

#ifndef YACK_COMPARISON_INCLUDED
#define YACK_COMPARISON_INCLUDED 1

#include "yack/type/utils.hpp"
#include "yack/type/abs.hpp"
#include "yack/signs.hpp"

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
        int increasing(const T &lhs, const T &rhs) noexcept
        {
            return (lhs<rhs) ? -1 : (rhs<lhs ? 1 : 0);
        }

        //______________________________________________________________________
        //
        //! increasing absolute value
        //______________________________________________________________________
        template <typename T> static inline
        int increasing_abs(const T &lhs, const T &rhs) noexcept
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
        int decreasing(const T &lhs, const T &rhs) noexcept
        {
            return (lhs<rhs) ? 1 : (rhs<lhs ? -1 : 0);
        }

        //______________________________________________________________________
        //
        //! increasing absolute value
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing_abs(const T &lhs, const T &rhs) noexcept
        {
            const T L = absolute(lhs);
            const T R = absolute(rhs);
            return (L<R) ? 1 : (R<L ? -1 : 0);
        }



        //______________________________________________________________________
        //
        //! test that a range is ordered
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
        //! lexicographic comparison of same lengths arrays[0..n-1]
        //______________________________________________________________________
        template <typename T> static inline
        int lexicographic(const T *lhs, const T *rhs, const size_t n) noexcept
        {
            assert(NULL!=lhs);
            assert(NULL!=rhs);
            for(size_t i=n;i>0;--i)
            {
                const T &L = *(lhs++);
                const T &R = *(rhs++);
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

        //______________________________________________________________________
        //
        //! equality of linearly accessible space
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        bool equality(LHS &lhs, RHS &rhs)
        {
            const size_t nl = lhs.size();
            const size_t nr = rhs.size();
            if(nl!=nr) return false;
            else {
                for(size_t i=nl;i>0;--i) { if(lhs[i]!=rhs[i]) return false; }
                return true;
            }
        }

        //______________________________________________________________________
        //
        //! disparity of linearly accessible space
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        bool disparity(LHS &lhs, RHS &rhs)
        {
            const size_t nl = lhs.size();
            const size_t nr = rhs.size();
            if(nl!=nr) return true;
            else {
                for(size_t i=nl;i>0;--i) { if(lhs[i]!=rhs[i]) return true; }
                return false;
            }
        }

        //______________________________________________________________________
        //
        //! generic lexicographic comparison
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        int lexicographic(LHS &lhs, RHS &rhs)
        {
            const size_t    nl = lhs.size();
            const size_t    nr = rhs.size();
            const sign_type d  = __sign::of(nl,nr);
            const size_t    ns = (d!=positive) ? nl : nr;

            assert(ns<=nl);
            assert(ns<=nr);
            assert(ns==nl || ns==nr);

            // look up on shared size
            for(size_t i=1;i<=ns;++i)
            {
                switch( __sign::of(lhs[i],rhs[i]) )
                {
                    case __zero__: continue;
                    case negative: return -1;
                    case positive: return  1;
                }
            }

            // all equal up to now: conclude
            switch(d)
            {
                case __zero__: break;
                case negative: assert(nl<nr); return -1;
                case positive: assert(nr<nl); return  1;
            }
            return 0;
        }



    };
    
}

#endif

