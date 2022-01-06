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
            template <typename T>
            class lambda : public field1D<T>
            {
            public:
                typedef field1D<T> field_type;
                
                static unit_t pmin() throw();
                static unit_t pmax() throw();

                explicit lambda();
                virtual ~lambda() throw();

                const unit_t p_in;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lambda);
            };

        }

    }

}

#endif
