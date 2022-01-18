
//! \file

#ifndef YACK_FIT_GLS_INCLUDED
#define YACK_FIT_GLS_INCLUDED 1

#include "yack/math/fit/least-squares.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {

            template <typename T>
            class gls : public least_squares<T,T>
            {
            public:
                explicit gls();
                virtual ~gls() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(gls);
            };

        }

    }

}

#endif
