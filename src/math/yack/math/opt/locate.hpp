
//! \file

#ifndef YACK_OPT_LOCATE_INCLUDED
#define YACK_OPT_LOCATE_INCLUDED 1

#include "yack/math/real-function.hpp"
#include "yack/math/triplet.hpp"

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! locate in 1D
        //
        //______________________________________________________________________
        struct locate
        {
            //__________________________________________________________________
            //
            //! trigger verbosity
            //__________________________________________________________________
            static bool verbose;

            //! guess with x.a <= x.b <= x.c, f.b <= f.a, f.b <= f.c
            /**
             if(x.b<=x.a or x.c<=x.a) return (x.a+x.b)/2
             */
            template <typename T> T guess(const triplet<T> &x, const triplet<T> &y) throw();

        };

    }

}

#endif
