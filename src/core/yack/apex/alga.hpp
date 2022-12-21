//! \file

#ifndef YACK_APEX_ALGA_INCLUDED
#define YACK_APEX_ALGA_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{

    //______________________________________________________________________
    //
    //
    //! Apex Library of Generic Algorithm
    //
    //______________________________________________________________________
    struct alga
    {
        //______________________________________________________________________
        //
        //! find least common multiple of an array of rationals
        //______________________________________________________________________
        template <typename ARRAY, typename INDEX> static inline
        apn lcm(ARRAY &arr, const INDEX lo, const INDEX hi)
        {
            assert(lo<=hi);
            if(lo>=hi)
            {
                return arr[lo].den;
            }
            else
            {
                apn res = apn::lcm_(arr[lo].den,arr[lo+1].den);
                for(INDEX i=lo+2;i<=hi;++i)
                    res = apn::lcm_(res,arr[i].den);
                return res;
            }
        }

        //! simplify keeping signs
        static void simplify( writable<apq> &v );

        //! simplify to univocal
        static void univocal( writable<apq> &v );

        



    };



}

#endif
