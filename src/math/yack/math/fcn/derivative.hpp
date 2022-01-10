//! \file

#ifndef YACK_MATH_DERIVATIVE_INCLUDED
#define YACK_MATH_DERIVATIVE_INCLUDED 1

#include "yack/math/real-function.hpp"
#include "yack/container/matrix.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! first order derivative evaluation
        //
        //______________________________________________________________________
        template <typename T>
        class derivative : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr< derivative<T> > pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit derivative();          //!< setup, allocate matrix
            virtual ~derivative() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static T unit_step_size() throw(); //!< step scaling w.r.t characteristic scale of 1
            static T diff_maxi_ftol() throw(); //!< first order max fractional tolerance


            //__________________________________________________________________
            //
            //! estimate derivative by Ridder's method
            /**
             if x_c is the characteristic scaling of f variations, then
             h = x_c * unit_step_size() is a good choice
             */
            //__________________________________________________________________
            T diff_(real_function<T> &f, const T x, T h, T &err);


            //! estimate derivative by Ridder's method, wrapper
            template <typename FUNCTION> inline
            T diff(FUNCTION &f, const T x, T h, T &err)
            {
                typename real_function_of<T>:: template call<FUNCTION> _(f);
                return diff_(_,x,h,err);
            }

            //__________________________________________________________________
            //
            //! best effort first order derivative
            //__________________________________________________________________
            T diff_(real_function<T> &f, const T x, T h);


            //! best effort first order derivative, wrapper
            template <typename FUNCTION> inline
            T diff(FUNCTION &f, const T x, T h)
            {
                typename real_function_of<T>:: template call<FUNCTION> _(f);
                return diff_(_,x,h);
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(derivative);
            matrix<T> a;

            void regularize(const T x, T &h);
            void rescale(const T x, T &h);
            bool converged(T &ans, const size_t i, T &err) throw();



        };

    }

}

#endif
