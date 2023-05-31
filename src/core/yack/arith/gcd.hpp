//! \file


#ifndef YACK_ARITH_GCDXX_INCLUDED
#define YACK_ARITH_GCDXX_INCLUDED 1

#include "yack/arith/gcd.h"
#include "yack/type/abs.hpp"

namespace yack
{


    //__________________________________________________________________________
    //
    //
    //! C++ helpers for simplifications
    //
    //__________________________________________________________________________
    struct gcdxx
    {
        //! gcd of items that are not zeo
        template <typename ITER> static inline
        uint64_t of(ITER curr, const size_t n)
        {
            switch(n)
            {
                case 0: return 1;
                case 1: return *curr;
                default:
                    break;
            }

            for(size_t i=0;i<n;++i)
            {
                uint64_t g = absolute( *(curr++) );
                if(g<=0)
                    continue;
                for(++i;i<n;++i)
                {
                    const uint64_t rhs = absolute( *(curr++) );
                    if(rhs<=0) continue;
                    g = yack_gcd64_(g,rhs);
                }
                return g;
            }

            return 1; // all coefficient were 0
        }

        //! compute gcd and simplify
        template <typename ITER> static inline
        void simplify_(ITER curr, size_t n)
        {
            const uint64_t g = of(curr,n);
            while(n-- > 0) *(curr++)/=g;
        }

        //! simplify an integral array
        template <typename T> static inline
        void simplify(T *arr, const size_t num)
        {
            assert(yack_good(arr,num));
            simplify_(arr,num);
        }

        //! simplify an integral sequence
        template <typename SEQ> static inline
        void simplify(SEQ &seq)
        {
            simplify_(seq.begin(),seq.size());
        }


    };

}

#endif

