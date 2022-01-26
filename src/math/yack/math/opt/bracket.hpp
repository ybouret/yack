
//! \file

#ifndef YACK_OPT_BRACKET_INCLUDED
#define YACK_OPT_BRACKET_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/math/real-function.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! bracketing algorithm
        //
        //______________________________________________________________________
        struct bracket
        {

            //__________________________________________________________________
            //
            //! inside [x.a,x.c], with f.a and f.c computed
            /**
             - Look for f.b<=f.a and f.c <= f.c
             - f.b = f(x.b) is always the last evaluated value
             */
            //__________________________________________________________________
            template <typename T> static
            bool inside(real_function<T> &F, triplet<T> &x, triplet<T> &f);

            //__________________________________________________________________
            //
            //! wrapper for inside
            //__________________________________________________________________
            template <typename T, typename FUNCTION> static inline
            bool inside_for(FUNCTION &F, triplet <T> &x, triplet<T> &f)
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                return inside(FF,x,f);
            }

        };

    }

}

#endif

