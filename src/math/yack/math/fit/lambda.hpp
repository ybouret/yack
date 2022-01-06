//! \file

#ifndef YACK_FIT_LAMBDA_INCLUDED
#define YACK_FIT_LAMBDA_INCLUDED 1

#include "yack/field/in1d.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            //__________________________________________________________________
            //
            //
            //! precomputed factors
            //
            //__________________________________________________________________
            template <typename T>
            class lambda : public field1D<T>
            {
            public:
                typedef field1D<T> field_type; //!< alias
                
                static unit_t pmin() throw();  //!< ceil( log10(epsilon) )
                static unit_t pmax() throw();  //!< max_exp10

                explicit lambda();         //!< setup
                virtual ~lambda() throw(); //!< cleanup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lambda);
            };

        }

    }

}

#endif
