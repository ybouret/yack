//! \file

#ifndef YACK_COMPLEX_INCLUDED
#define YACK_COMPLEX_INCLUDED 1

#include "yack/setup.hpp"
#include <iostream>

namespace yack
{

    template <typename T>
    class complex
    {
    public:
        T re;
        T im;

        inline ~complex() throw() {}

        inline complex() throw() : re(0), im(0) {}
        inline complex(const T x) throw() : re(x), im(0) {}
        inline complex(const T x, const T y) throw() : re(x), im(y) {}
        inline complex(const complex &z) throw() : re(z.re), im(z.im) {}
        
        // assign
        inline complex & operator=(const complex &z) throw()
        {
            re=z.re; im=z.im; return *this;
        }

        inline complex & operator=(const T x) throw()
        {
            re = x; im = 0; return *this;
        }

        // ADD
        inline complex operator+() const { return *this; }
       
        inline friend complex operator+(const complex &lhs, const complex &rhs) throw()
        { return complex(lhs.re+rhs.re,lhs.im+rhs.im); }
        
        inline friend complex operator+(const complex &lhs, const T rhs) throw()
        { return complex(lhs.re+rhs,lhs.im); }
        
        inline friend complex operator+(const T lhs, const complex &rhs) throw()
        { return complex(lhs+rhs.re,rhs.im); }
        
        inline complex & operator+=(const complex &z) throw()
        { re += z.re; im += z.im; return *this; }
        
        inline complex & operator+=(const T x) throw()
        { re += x; return *this; }

        // SUB
        inline complex operator-() const { return complex(-re,-im); }

        inline friend complex operator-(const complex &lhs, const complex &rhs) throw()
        { return complex(lhs.re-rhs.re,lhs.im-rhs.im); }
 
        inline friend complex operator-(const complex &lhs, const T rhs) throw()
        { return complex(lhs.re+rhs,lhs.im); }
        
        inline friend complex operator-(const T lhs, const complex &rhs) throw()
        { return complex(lhs-rhs.re,-rhs.im); }
        
        inline complex & operator-=(const complex &z) throw()
        { re -= z.re; im -= z.im; return *this; }
        
        inline complex & operator-=(const T x) throw()
        { re -= x; return *this; }

        // MUL
        inline complex & operator*=(const complex &z) throw()
        {
            const T rere = re * z.re;
            const T imim = im * z.im;
            im = (re+im)*(z.re+z.im) - rere - imim;
            re = rere - imim;
            return *this;
        }
        
        

        friend inline std::ostream &operator<<(std::ostream &os, const complex &z)
        {
            return os << '(' << z.re << ',' << z.im << ')';
        }

    };
}

#endif

