//! \file

#ifndef YACK_MATH_FIT_SEQUENTIAL_INCLUDED
#define YACK_MATH_FIT_SEQUENTIAL_INCLUDED 1

#include "yack/math/fit/variables.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            //__________________________________________________________________
            //
            //
            //! sequential function interface
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sequential_function
            {
            public:
                //! cleanup
                inline virtual ~sequential_function() throw() {}

                //! initialize call
                inline ORDINATE start(const ABSCISSA ini, const readable<ORDINATE> &A, const variables &vars)
                {
                    return on_start(saved=ini,A,vars);
                }

                //! reach next value
                inline ORDINATE reach(const ABSCISSA end, const readable<ORDINATE> &A, const variables &vars)
                {
                    const ABSCISSA org = saved;
                    return on_reach(org,saved=end,A,vars);
                }

            protected:
                inline explicit sequential_function() throw() : saved() {} //!< setup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sequential_function);
                virtual ORDINATE on_start(const ABSCISSA ini, const readable<ORDINATE> &A, const variables &vars) = 0;
                virtual ORDINATE on_reach(const ABSCISSA ini, const ABSCISSA end, const readable<ORDINATE> &A, const variables &vars) = 0;
                ABSCISSA saved;
            };


            //__________________________________________________________________
            //
            //
            //! sequential gradient interface
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sequential_gradient
            {
            public:
                //! cleanup
                inline virtual ~sequential_gradient() throw() {}

                

            protected:
                inline explicit sequential_gradient() throw() {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sequential_gradient);
            };
        }

    }
}

#endif

