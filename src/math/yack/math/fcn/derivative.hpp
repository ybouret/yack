

//! \file

#ifndef YACK_MATH_DERIVATIVE_INCLUDED
#define YACK_MATH_DERIVATIVE_INCLUDED 1

#include "yack/container/matrix.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        template <typename T>
        class derivative
        {
        public:
            virtual ~derivative() throw();
            explicit derivative();



            static T unit_step_size() throw(); //!< step scaling w.r.t characteristic scale of 1
            static T diff_maxi_ftol() throw(); //!< first order max fractional tolerance

            //! estimate derivative by Ridder's method
            /**
             if x_c is the characteristic scaling of f variations, then
             h = x_c * unit_step_size() is a good choice
             */
            template <typename FUNCTION> inline
            T diff(FUNCTION &f, const T x, T h, T &err)
            {
                const size_t dim = a.rows; regularize(x,h);
                T            ans = (a[1][1] = ( f(x+h)-f(x-h) ) / (h+h));
                for(size_t i=2;i<=dim;++i)
                {
                    rescale(x,h);
                    a[1][i] = ( f(x+h)-f(x-h) ) / (h+h);
                    if(converged(ans,i,err)) return ans;
                }
                return ans;
            }

            //! best effort first order derivative
            template <typename FUNCTION> inline
            T diff(FUNCTION &f, const T x, T h )
            {
                static const T max_ftol = diff_maxi_ftol();
                T err  = 0;
                T dFdx = diff(f,x,h,err);
                while(err>max_ftol*fabs(dFdx) )
                {
                    T       new_err  = 0;
                    const T new_dFdx = diff(f,x,h/=2,new_err);
                    if(new_err>=err)
                    {
                        break; // not better
                    }
                    err  = new_err;
                    dFdx = new_dFdx;
                }
                return dFdx;
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
