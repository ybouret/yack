//! \file

#ifndef YACK_COMPLEX_INCLUDED
#define YACK_COMPLEX_INCLUDED 1

#include "yack/setup.hpp"
#include <iostream>

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
        typedef T real_t; //!< base type

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
        inline ~complex() throw() {}                                      //!< cleanup
        inline  complex() throw() : re(0), im(0) {}                       //!< 0
        inline  complex(const T x) throw() : re(x), im(0) {}              //!< x
        inline  complex(const T x, const T y) throw() : re(x), im(y) {}   //!< (x,y)
        inline  complex(const complex &z) throw() : re(z.re), im(z.im) {} //!< copy
        
        //! assign another complex
        inline complex & operator=(const complex &z) throw()
        {
            re=z.re; im=z.im; return *this;
        }

        //! assign a real
        inline complex & operator=(const T x) throw()
        {
            re = x; im = 0; return *this;
        }

        //______________________________________________________________________
        //
        // ADD
        //______________________________________________________________________
        inline complex operator+() const { return *this; } //!< unary+
       
        inline friend complex operator+(const complex &lhs, const complex &rhs) throw()
        { return complex(lhs.re+rhs.re,lhs.im+rhs.im); } //!<   z+z
        
        inline friend complex operator+(const complex &lhs, const T rhs) throw()
        { return complex(lhs.re+rhs,lhs.im); } //!< z+x
        
        inline friend complex operator+(const T lhs, const complex &rhs) throw()
        { return complex(lhs+rhs.re,rhs.im); } //!< x+z
        
        inline complex & operator+=(const complex &z) throw()
        { re += z.re; im += z.im; return *this; } //!< += z
        
        inline complex & operator+=(const T x) throw()
        { re += x; return *this; } //!< += x

        //______________________________________________________________________
        //
        // SUB
        //______________________________________________________________________
        inline complex operator-() const { return complex(-re,-im); } //!< unary-

        inline friend complex operator-(const complex &lhs, const complex &rhs) throw()
        { return complex(lhs.re-rhs.re,lhs.im-rhs.im); } //!< z-z
 
        inline friend complex operator-(const complex &lhs, const T rhs) throw()
        { return complex(lhs.re+rhs,lhs.im); } //!< z-x
        
        inline friend complex operator-(const T lhs, const complex &rhs) throw()
        { return complex(lhs-rhs.re,-rhs.im); } //!< x-z
        
        inline complex & operator-=(const complex &z) throw()
        { re -= z.re; im -= z.im; return *this; } //!< -= z
        
        inline complex & operator-=(const T x) throw()
        { re -= x; return *this; } //!< -= x

        //______________________________________________________________________
        //
        //! conjugaison
        //______________________________________________________________________
        inline complex conj() const throw() { return complex(re,-im); }


        //______________________________________________________________________
        //
        // MUL
        //______________________________________________________________________
        //! *= z
        inline complex & operator*=(const complex &z) throw()
        {
            const T rere = re * z.re;
            const T imim = im * z.im;
            im = (re+im)*(z.re+z.im) - rere - imim;
            re = rere - imim;
            return *this;
        }
        
        //! z*z
        inline friend complex operator * (const complex &lhs, const complex &rhs) throw()
        {
            const T rere = lhs.re * rhs.re;
            const T imim = lhs.im * rhs.im;
            return complex(rere-imim,(lhs.re+lhs.im)*(rhs.re+rhs.im) - rere - imim);
        }
        
        //! x*z
        inline friend complex operator * (const T lhs, const complex &rhs) throw()
        {
            return complex(lhs*rhs.re,lhs*rhs.im);
        }
        
        //! z*x
        inline friend complex operator * (const complex &lhs, const T rhs) throw()
        {
            return complex(lhs.re*rhs,lhs.im*rhs);
        }


        //______________________________________________________________________
        //
        //! mod2
        //______________________________________________________________________
        inline real_t mod2() const throw() { return re*re+im*im; }

        
        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        friend inline std::ostream &operator<<(std::ostream &os, const complex &z)
        {
            return os << '(' << z.re << ',' << z.im << ')';
        }

    };
}

#endif

