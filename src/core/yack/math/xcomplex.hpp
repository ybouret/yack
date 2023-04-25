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
        typedef T           type; //!< alias
        typedef extended<T> xflt; //!< alias
        typedef complex<T>  cplx; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        xcomplex()  noexcept;                              //!< 0
        xcomplex(const xcomplex &) noexcept;               //!< copy
        xcomplex &operator=(const xcomplex &) noexcept;    //!< assign
        ~xcomplex() noexcept;                              //!< cleanup

        xcomplex(const type  x);                           //!< (x,0)
        xcomplex(const xflt  X) noexcept;                  //!< (X,0)
        xcomplex(const cplx  z);                           //!< (z.re,z.im)

        //! special constructor (u,v)
        template <typename U, typename V> inline
        xcomplex(const U u, const V v) : re(u), im(v) {}

        xcomplex & operator=(const type x);               //!< (x,0)
        xcomplex & operator=(const xflt X) noexcept;      //!< (X,0)
        xcomplex & operator=(const cplx z);               //!< (z.re,z.im)

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        xflt     mod2() const;          //!< |z|^2
        xcomplex conj() const noexcept; //!< (re,-im)
        cplx     operator*() const;

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
        xcomplex operator+() const noexcept; //!< unary +
        xcomplex operator-() const noexcept; //!< unary -



        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________
        xcomplex & operator+=(const xcomplex rhs); //!< binary+
        inline friend xcomplex operator+(xcomplex lhs, const xcomplex rhs) {
            return lhs += rhs;
        }



        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________
        xcomplex & operator-=(const xcomplex rhs); //!< binary-
        inline friend xcomplex operator-(xcomplex lhs, const xcomplex rhs) {
            return lhs -= rhs;
        }




        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        xcomplex & operator*=(const xcomplex rhs);
        inline friend xcomplex operator*(xcomplex lhs, const xcomplex rhs) {
            return lhs *= rhs;
        }

        

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        xcomplex & operator/=(const xcomplex den);
        inline friend xcomplex operator/(xcomplex lhs, const xcomplex rhs) {
            return lhs /= rhs;
        }



        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        xflt re; //!< real part
        xflt im; //!< imaginary part
    };

    //! specific scalar type
    template <typename T>
    struct scalar_for< xcomplex<T> >
    {
        typedef typename xcomplex<T>::xflt type; //!< effective type
    };

}

#endif

