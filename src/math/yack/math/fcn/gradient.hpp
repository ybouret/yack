
//! \file

#ifndef YACK_MATH_GRADIENT_INCLUDED
#define YACK_MATH_GRADIENT_INCLUDED 1

#include "yack/math/fcn/derivative.hpp"
#include "yack/functor.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class gradient
        {
        public:
            typedef typename derivative<T>::pointer derivative_ptr;

            inline virtual ~gradient() throw() {}
            inline explicit gradient(const derivative_ptr &handle) throw() :
            drvs(handle)
            {
            }

            derivative_ptr drvs;

            template <typename OUTPUT, typename FUNCTION, typename INPUT> inline
            void compute( OUTPUT &g, FUNCTION &f, INPUT &x )
            {
                
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gradient);
        };

    }

}

#endif
