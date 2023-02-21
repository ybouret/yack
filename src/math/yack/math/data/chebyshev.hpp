
//! \file

#ifndef YACK_MATH_CHEBYSHEV_INCLUDED
#define YACK_MATH_CHEBYSHEV_INCLUDED 1

#include "yack/math/numeric.hpp"
#include "yack/sequence/arrays.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! Chebyshev approximations
        //
        //______________________________________________________________________
        template <typename T>
        class chebyshev : public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            typedef arrays_of<T>                  tableaux;   //!< alias
            typedef typename tableaux::array_type array_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit chebyshev();          //!< setup
            virtual ~chebyshev() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! build coefficients for functions F(x), x in [a:b]
            template <typename FUNC>
            void build(FUNC &func, const T a, const T b, const size_t n)
            {
                assert(n>0);
                load(func,a,b,n);
                coef(n);
            }

            //! evaluate at x with order m<=n (mutual_size())
            T at(const T x, const T a, const T b, const size_t m) const noexcept;
            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chebyshev);
            array_type &c;
            array_type &f;
            array_type &p;

            void coef(const size_t) noexcept;

            template <typename FUNC> inline
            void load(FUNC       &func,
                      const T      a,
                      const T      b,
                      const size_t n)
            {
                static const T PI   = numeric<T>::pi;
                static const T half(0.5);

                this->make(n);
                const T bma=half*(b-a);
                const T bpa=half*(b+a);
                for(size_t k=0;k<n;)
                {
                    const T x = PI*(k+half)/n;
                    const T y = cos(x);
                    f[++k]    = func(y*bma+bpa);
                }
            }
        };

    }

}

#endif
