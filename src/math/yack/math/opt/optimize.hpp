
//! \file

#ifndef YACK_OPT_OPTIMIZE_INCLUDED
#define YACK_OPT_OPTMIZE_INCLUDED 1

#include "yack/math/real-function.hpp"
#include "yack/math/triplet.hpp"

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! optimize in 1D
        //
        //______________________________________________________________________
        struct optimize
        {
            //__________________________________________________________________
            //
            //! trigger verbosity
            //__________________________________________________________________
            static bool verbose;

            //! guess with x.a <= x.b <= x.c, f.b <= f.a, f.b <= f.c

            template <typename T> static
            T parabolic_guess(const triplet<T> &x, const triplet<T> &f) throw();

        };

    }

}

#endif
