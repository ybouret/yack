//! \file

#ifndef YACK_MATH_XCOMPLEX_INCLUDED
#define YACK_MATH_XCOMPLEX_INCLUDED 1

#include "yack/math/extended.hpp"
#include "yack/type/complex.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! extended complex
    //
    //__________________________________________________________________________
    template <typename T>
    class xcomplex
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef extended<T> xreal; //!< alias
        typedef complex<T>  cplx;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        xcomplex()  noexcept;                              //!< 0
        xcomplex(const xcomplex &) noexcept;               //!< copy
        xcomplex &operator=(const xcomplex &) noexcept;    //!< assign
        ~xcomplex() noexcept;                              //!< cleanup

        xcomplex(const T     x);                           //!< (x,0)
        xcomplex(const xreal X) noexcept;                  //!< (X,0)
        xcomplex(const cplx  z);                           //!< (z.re,z.im)

        //! special constructor (u,v)
        template <typename U, typename V> inline
        xcomplex(const U u, const V v) : re(u), im(v) {}

        xcomplex & operator=(const T     x);               //!< (x,0)
        xcomplex & operator=(const xreal X) noexcept;      //!< (X,0)
        xcomplex & operator=(const cplx  z);               //!< (z.re,z.im)

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        xreal    mod2() const;          //!< |z|^2
        xcomplex conj() const noexcept; //!< (re,-im)


        //! display
        inline friend std::ostream & operator<<(std::ostream   &os,
                                                const xcomplex &z)
        {
            os << '(' << z.re << ',' << z.im << ')';
            return os;
        }

        //______________________________________________________________________
        //
        // unary operators
        //______________________________________________________________________
        xcomplex operator+() const noexcept;
        xcomplex operator-() const noexcept;


        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________
        xcomplex & operator+=(const xcomplex rhs);
        inline friend xcomplex operator+(xcomplex lhs, const xcomplex rhs )
        {
            return lhs += rhs;
        }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________
        xcomplex & operator-=(const xcomplex rhs);
        inline friend xcomplex operator-(xcomplex lhs, const xcomplex rhs )
        {
            return lhs -= rhs;
        }

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        xcomplex & operator*=(const xcomplex rhs);
        inline friend xcomplex operator*(xcomplex lhs, const xcomplex rhs )
        {
            return lhs *= rhs;
        }

        xcomplex & operator*=(const xreal rhs);
        inline friend xcomplex operator*(xcomplex lhs, const xreal rhs )
        {
            return lhs *= rhs;
        }
        inline friend xcomplex operator*(const xreal lhs, xcomplex rhs )
        {
            return rhs *= lhs;
        }

        xcomplex & operator*=(const T rhs);
        inline friend xcomplex operator*(xcomplex lhs, const T rhs )
        {
            return lhs *= rhs;
        }

        inline friend xcomplex operator*(const T lhs, xcomplex rhs )
        {
            return rhs *= lhs;
        }

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        xcomplex & operator/=(const xcomplex den);




        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        xreal re; //!< real part
        xreal im; //!< imaginary part
    };

}

#endif

