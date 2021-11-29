
//! \file

#ifndef YACK_OPT_BRACKET_INCLUDED
#define YACK_OPT_BRACKET_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/math/numeric.hpp"
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

            //! inside [x.a,x.c], with f.a and f.c computed
            /**
             Look for f.b<=f.a and f.c <= f.c
             */
            template <typename T, typename FUNCTION> static inline
            bool inside(triplet <T> &x, triplet<T> &f, FUNCTION &F)
            {
                static const T r = numeric<T>::golden_r;

                //--------------------------------------------------------------
                //
                // going downwards from a to c
                //
                //--------------------------------------------------------------
                if(f.c>f.a)
                {
                    cswap(f.a,f.c);
                    cswap(x.a,x.c);
                }

                T width = fabs(x.c-x.a);
            CYCLE:
                assert(f.c<=f.a);
                // put b closer to the smallest value
                f.b     = F(x.b =  x.a + r * (x.c-x.a)); assert(x.is_ordered());
                if(f.b<=f.c)
                {
                    assert(f.b<=f.c); // since f.c <= f.a
                    return true;
                }
                else
                {
                    x.a = x.b;
                    f.a = f.b;
                    const T new_width = fabs(x.c-x.a);
                    if(new_width>=width) return false;
                    width = new_width;
                    goto CYCLE;
                }


            }

        };

    }

}

#endif

