//! \file

#ifndef YACK_FITTING_LAMBDA_INCLUDED
#define YACK_FITTING_LAMBDA_INCLUDED 1

#include "yack/field/in1d.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {



            //__________________________________________________________________
            //
            //
            //! precomputed powers of ten for a given floating type
            //
            //__________________________________________________________________
            template <typename T>
            class lambda : public field1D<const T>
            {
            public:
                typedef field1D<const T> field_type;

                static const char *clid() throw(); //!< lambda<T>
                static int         pmin() throw(); //!< ceil( log10( epsilon ) )
                static int         pmax() throw(); //!< MAX_10_EXP

                virtual ~lambda() throw();
                explicit lambda();
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lambda);
            };

        }
    }
}

#endif

