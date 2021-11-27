//! \file

#ifndef YACK_MATH_GRADIENT_INCLUDED
#define YACK_MATH_GRADIENT_INCLUDED 1

#include "yack/math/fcn/derivative.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! numerical gradient from derivative
        //
        //______________________________________________________________________
        template <typename T>
        class gradient
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename derivative<T>::pointer derivative_ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~gradient() throw();
            
            //! setup
            explicit gradient(const derivative_ptr &, const T = 1e-4) throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! generic computation
            template <typename OUTPUT, typename FUNCTION, typename INPUT> inline
            void compute(OUTPUT &g, FUNCTION &f, INPUT &x)
            {
                assert(g.size()==x.size());
                struct proxy<FUNCTION,INPUT> func = { 0, f, x };
                size_t                      &i    = func.i;
                for(i=x.size();i>0;--i)
                    g[i] = drvs->diff(func,x[i],scal);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            derivative_ptr drvs; //!< 1D numerical derivative
            T              scal; //!< initial scaling
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gradient);
            template <typename FUNCTION, typename INPUT>
            struct proxy
            {
                size_t    i;
                FUNCTION &f;
                INPUT    &x;
                inline T operator()(typename INPUT::type xx)
                {
                    temporary<typename INPUT::mutable_type> xsav( coerce(x[i]), xx );
                    return f(x);
                }
            };
        };

    }

}

#endif
