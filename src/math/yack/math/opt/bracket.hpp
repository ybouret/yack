
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
             - Look for f.b<=f.a and f.b <= f.c
             - f.b = f(x.b) is always the last evaluated value
             - can be on a side of the interval
             */
            //__________________________________________________________________
            template <typename T> static
            void inside(real_function<T> &F, triplet<T> &x, triplet<T> &f);


            //__________________________________________________________________
            //
            //! wrapper for inside
            //__________________________________________________________________
            template <typename T, typename FUNCTION> static inline
            void inside_for(FUNCTION &F, triplet <T> &x, triplet<T> &f)
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                inside(FF,x,f);
            }


            //__________________________________________________________________
            //
            //! expand ordered [x.a,x.b,x.c], with [f.a,f.b,f.c] computed
            /**

             */
            //__________________________________________________________________
            template <typename T> static
            void expand(real_function<T> &F, triplet<T> &x, triplet<T> &f);

            //__________________________________________________________________
            //
            //! wrapper for expand
            //__________________________________________________________________
            template <typename T, typename FUNCTION> static inline
            void expand_for(FUNCTION &F, triplet <T> &x, triplet<T> &f)
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                expand(FF,x,f);
            }

        };

    }

}

#endif

