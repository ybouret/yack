//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fit/sample.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {

            template <
            typename ABSCISSA,
            typename ORDINATE>
            class least_squares 
            {
            public:
                typedef sample<ABSCISSA,ORDINATE>              sample_type;
                typedef typename derivative<ORDINATE>::pointer drvs_type;

                inline virtual ~least_squares() throw() {}
                inline explicit least_squares() throw() {}

                


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);
            };

        }
    }

}

#endif

