
//! \file

#ifndef YACK_APEX_KERNEL_INCLUDED
#define YACK_APEX_KERNEL_INCLUDED 1

#include "yack/apex.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! apex kernel
    //
    //__________________________________________________________________________
    struct apk
    {

        //! find least common multiple of an array of rationals
        template <typename ARRAY, typename INDEX> static inline
        apn lcm(const ARRAY &arr, const INDEX lo, const INDEX hi)
        {
            assert(lo<=hi);
            if(lo>=hi)
            {
                return arr[lo].den;
            }
            else
            {
                apn res = apn::lcm(arr[lo].den,arr[lo+1].den);
                for(INDEX i=lo+2;i<=hi;++i)
                {
                    res = apn::lcm(res,arr[i].den);
                }
                return res;
            }
        }

        //! find lcm for a sequence of rationals
        template <typename SEQUENCE> static inline
        apn lcm(const SEQUENCE &seq)
        {
            return lcm<SEQUENCE,size_t>(seq,1,seq.size());
        }

        //! find lcm for a 1D field
        template <typename FIELD> static inline
        apn lcm1D(const FIELD &F)
        {
            return lcm<FIELD,unit_t>(F,F.lower,F.upper);
        }

        //! find lcm for a 2D field/matrix or rationals
        template <typename FIELD,typename INDEX> static inline
        apn lcm(const FIELD &F, const INDEX xlo, const INDEX xhi,
                const INDEX ylo, const INDEX yhi)
        {
            assert(xhi>=xlo);
            assert(yhi>=ylo);
            unit_t y   = yhi;
            apn    res = lcm1D(F[y]);
            for(--y;y>=ylo;--y)
            {
                const apn tmp = lcm(F[y],xlo,xhi);
                res = apn::lcm(res,tmp);
            }
            return res;
        }


        //! find lcm for a 2D field of rationals
        template <typename FIELD> static inline
        apn lcm2D(const FIELD &F)
        {
            return lcm<FIELD,unit_t>(F,F.lower.x,F.upper.x,F.lower.y,F.upper.y);
        }


    };

}

#endif
