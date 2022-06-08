
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

            template <typename T> T guess(const triplet<T> &x, const triplet<T> &y) throw();

        };

    }

}

#endif
