
//! \file

#ifndef YACK_OPT_BRACKET_INCLUDED
#define YACK_OPT_BRACKET_INCLUDED 1

#include "yack/math/triplet.hpp"

namespace yack
{

    namespace math
    {

        struct bracket
        {

            //! inside [x.a,x.c], with f.a and f.c computed
            template <typename T, typename FUNCTION> static inline
            void inside(triplet <T> &x, triplet<T> &f, FUNCTION &F)
            {
                static const T half(0.5);

                f.b = F( x.b = half*(x.a+x.c) );
                

            }

        };

    }

}

#endif

