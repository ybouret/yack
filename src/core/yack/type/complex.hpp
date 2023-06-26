//! \file

#ifndef YACK_COMPLEX_INCLUDED
#define YACK_COMPLEX_INCLUDED 1

#include "yack/math/hypothenuse.hpp"
#include "yack/type/utils.hpp"
#include <iostream>
#include <cmath>

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! complex class
    //
    //__________________________________________________________________________
    template <typename T>
    class complex
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef T type; //!< base type

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        T re; //!< real part
        T im; //!< imaginary part

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline ~complex() noexcept {}                                      //!< cleanup
        inline  complex() noexcept : re(0), im(0) {}                       //!< 0
        inline  complex(const T x) noexcept : re(x), im(0) {}              //!< x
        inline  complex(const T x, const T y) noexcept : re(x), im(y) {}   //!< (x,y)
        inline  complex(const complex &z) noexcept : re(z.re), im(z.im) {} //!< copy
        
        //! assign another complex
        inline complex & operator=(const complex &z) noexcept
        {
            re=z.re; im=z.im; return *this;
        }

        //! assign a real
        inline complex & operator=(const T x) noexcept
        {
            re = x; im = 0; return *this;
        }

        //______________________________________________________________________
        //
        // ADD
        //______________________________________________________________________
        inline complex operator+() const { return *this; } //!< unary+
       
        inline friend complex operator+(const complex &lhs, const complex &rhs) noexcept
        { return complex(lhs.re+rhs.re,lhs.im+rhs.im); } //!<   z+z
        
        inline friend complex operator+(const complex &lhs, const T rhs) noexcept
        { return complex(lhs.re+rhs,lhs.im); } //!< z+x
        
        inline friend complex operator+(const T lhs, const complex &rhs) noexcept
        { return complex(lhs+rhs.re,rhs.im); } //!< x+z
        
        inline complex & operator+=(const complex &z) noexcept
        { re += z.re; im += z.im; return *this; } //!< += z
        
        inline complex & operator+=(const T x) noexcept
        { re += x; return *this; } //!< += x

        //______________________________________________________________________
        //
        // SUB
        //______________________________________________________________________
        inline complex operator-() const { return complex(-re,-im); } //!< unary-

        inline friend complex operator-(const complex &lhs, const complex &rhs) noexcept
        { return complex(lhs.re-rhs.re,lhs.im-rhs.im); } //!< z-z
 
        inline friend complex operator-(const complex &lhs, const T rhs) noexcept
        { return complex(lhs.re+rhs,lhs.im); } //!< z-x
        
        inline friend complex operator-(const T lhs, const complex &rhs) noexcept
        { return complex(lhs-rhs.re,-rhs.im); } //!< x-z
        
        inline complex & operator-=(const complex &z) noexcept
        { re -= z.re; im -= z.im; return *this; } //!< -= z
        
        inline complex & operator-=(const T x) noexcept
        { re -= x; return *this; } //!< -= x

        //______________________________________________________________________
        //
        //! conjugaison
        //______________________________________________________________________
        inline complex conj() const noexcept { return complex(re,-im); }


        //______________________________________________________________________
        //
        // MUL
        //______________________________________________________________________
        //! *= z
        inline complex & operator*=(const complex &z) noexcept
        {
            const T rere = re * z.re;
            const T imim = im * z.im;
            im = (re+im)*(z.re+z.im) - rere - imim;
            re = rere - imim;
            return *this;
        }

    private:
        static inline void aswap(T &lhs, T &rhs) noexcept
        {
            if( std::abs(rhs) < std::abs(lhs) ) cswap(lhs,rhs);
        }
    public:

        //! z1*z2
        inline friend complex operator * (const complex &lhs, const complex &rhs) noexcept
        {
            const T rere = lhs.re * rhs.re;
            const T imim = lhs.im * rhs.im;
            return complex(rere-imim,(lhs.re+lhs.im)*(rhs.re+rhs.im) - rere - imim);
#if 0
            T       arr[3] = {(lhs.re+lhs.im)*(rhs.re+rhs.im), -rere, -imim };
            aswap(arr[0],arr[2]);
            aswap(arr[0],arr[1]);
            aswap(arr[1],arr[2]);
            assert( std::abs(arr[0]) <= std::abs(arr[1]) );
            assert( std::abs(arr[1]) <= std::abs(arr[2]) );
            return complex(rere-imim,(arr[0]+arr[1]) + arr[2]);
#endif
        }
        
        //! x*z
        inline friend complex operator * (const T lhs, const complex &rhs) noexcept
        {
            return complex(lhs*rhs.re,lhs*rhs.im);
        }
        
        //! z*x
        inline friend complex operator * (const complex &lhs, const T rhs) noexcept
        {
            return complex(lhs.re*rhs,lhs.im*rhs);
        }


        //______________________________________________________________________
        //
        //! mod2
        //______________________________________________________________________
        inline type mod2() const noexcept { return squared(re) + squared(im); }
       
        //______________________________________________________________________
        //
        //! mod2
        //______________________________________________________________________
        inline type abs() const noexcept { return math::hypothenuse(im,re); }

        
        //______________________________________________________________________
        //
        // DIV
        //______________________________________________________________________
        //! z1/z2
        inline friend complex operator / (const complex &lhs, const complex &rhs) noexcept
        {
            return (lhs*rhs.conj())/rhs.mod2();
        }

        ///! z1 /= z2
        inline complex & operator/=(const complex &rhs) noexcept
        {
            const complex res = (*this) / rhs;
            return ( (*this) = (*this) / rhs);
        }

        //! z/x
        inline friend complex operator / (const complex &lhs, const T rhs ) noexcept
        {
            return complex(lhs.re/rhs,lhs.im/rhs);
        }

        //! z /= x
        inline complex & operator/=(const T rhs) noexcept
        {
            re/=rhs;
            im/=rhs;
            return *this;
        }

        //! x / z
        inline friend complex operator / (const T lhs, const complex &rhs) noexcept
        {
            return complex(lhs,0) / rhs;
        }

        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        friend inline std::ostream &operator<<(std::ostream &os, const complex &z)
        {
            return os << '(' << z.re << ',' << z.im << ')';
        }


        //! pseudo-collection
        size_t size() const noexcept { return 2; }

        //! pseudo writable
        T &operator[](size_t indx) noexcept
        {
            assert(indx>=1);assert(indx<=2);
            return *(&re + --indx);
        }

        //! pseudo readable
        const T &operator[](size_t indx) const noexcept
        {
            assert(indx>=1);assert(indx<=2);
            return *(&re + --indx);
        }


    };

    //! specific scalar type
    template <typename T>
    struct scalar_for< complex<T> >
    {
        typedef typename complex<T>::type type; //!< effective type
    };

}

#endif

