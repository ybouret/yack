
//! \file

#ifndef YACK_FIT_PRIMARY_INCLUDED
#define YACK_FIT_PRIMARY_INCLUDED 1

#include "yack/math/fit/variable.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            class primary : public variable
            {
            public:
                virtual ~primary() throw();

                template <typename ID>
                explicit primary(const ID &i, const size_t p) :
                variable(i),
                position(p)
                {
                }
                
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(primary);
                virtual size_t get_index() const throw();
                const size_t position;
            };
        }
    }
}

#endif


