//! \file

#ifndef YACK_FIT_EXPLODE_INCLUDED
#define YACK_FIT_EXPLODE_INCLUDED 1

#include "yack/math/fit/sequential.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            template <typename T> class explODE : public sequential<T,T>
            {
            public:
                typedef sequential<T,T> sequential_type;

                virtual ~explODE() throw();

            protected:
                explicit explODE();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(explODE);
            };


        }

    }
}


#endif

