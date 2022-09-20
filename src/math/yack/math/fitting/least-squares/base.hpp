//! \file

#ifndef YACK_FIT_LEAST_SQUARES__INCLUDED
#define YACK_FIT_LEAST_SQUARES__INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //! least square basic class
            class least_squares_
            {
            public:
                static const char   clid[];                      //!< "[least-squares]"
                static const char * ok(const bool flag) throw(); //!< textual

                virtual ~least_squares_() throw();
                explicit least_squares_() throw();


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares_);
            };


        }
    }
}

#endif
