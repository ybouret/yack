

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

            template <typename FUNCTION>
            T operator()(FUNCTION &f, const T x, T h, T &err)
            {
                const size_t ntab = a.rows;
                err               = -1;
                regularize(x,h);
                T ans = a[1][1] = ( f(x+h)-f(x-h) ) / (h+h);
                for(size_t i=2;i<=ntab;++i)
                {
                    rescale(x,h);
                    a[1][i] = ( f(x+h)-f(x-h) ) / (h+h);
                    if(converged(ans,i,err)) return ans;
                }
                return ans;
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
