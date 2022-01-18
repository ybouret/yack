
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

            //__________________________________________________________________
            //
            //
            //! specialized general least squares
            //
            //__________________________________________________________________
            template <typename T>
            class gls : public least_squares<T,T>
            {
            public:
                explicit gls();         //!< setup
                virtual ~gls() throw(); //!< cleanup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(gls);
            };

        }

    }

}

#endif
