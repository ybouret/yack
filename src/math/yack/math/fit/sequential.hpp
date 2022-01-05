//! \file

#ifndef YACK_MATH_FIT_SEQUENTIAL_INCLUDED
#define YACK_MATH_FIT_SEQUENTIAL_INCLUDED 1

#include "yack/math/fit/variables.hpp"
#include "yack/functor.hpp"

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
            class sequential
            {
            public:
                typedef readable<ORDINATE> parameters; //!< alias
                typedef writable<ORDINATE> field_type; //!< alias
                typedef readable<bool>     flags_type; //!< alias

                //! gradient interface definition
                typedef functor<void,TL5(field_type &,ABSCISSA,const parameters &,const variables &,const flags_type &)> gradient;

                //! cleanup
                inline virtual ~sequential() throw() {}

                //! initialize call
                inline ORDINATE start(const ABSCISSA ini, const parameters &A, const variables &vars)
                {
                    return on_start(saved=ini,A,vars);
                }

                //! reach next value
                inline ORDINATE reach(const ABSCISSA end, const parameters &A, const variables &vars)
                {
                    const ABSCISSA org = saved;
                    return on_reach(org,saved=end,A,vars);
                }


            protected:
                inline explicit sequential() throw() : saved() {} //!< setup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sequential);
                virtual ORDINATE on_start(const ABSCISSA ini, const parameters &A, const variables &vars) = 0;
                virtual ORDINATE on_reach(const ABSCISSA ini, const ABSCISSA end, const parameters &A, const variables &vars) = 0;
                ABSCISSA saved;
            };



        }

    }
}

#endif

