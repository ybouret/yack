
//! \file

#ifndef YACK_TYPE_V2D_INCLUDED
#define YACK_TYPE_V2D_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/math/hypothenuse.hpp"
#include "yack/comparison.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! 2D vertex
    //
    //__________________________________________________________________________
    template <typename T>
    class v2d
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type); //!< aliases

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        type x; //!< x
        type y; //!< y


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  v2d() noexcept : x(0), y(0) {}                           //!< setup
        inline ~v2d() noexcept {}                                        //!< cleanup
        inline  v2d(const v2d &v) noexcept : x(v.x), y(v.y) {}           //!< copy
        inline  v2d(const_type X, const_type Y) noexcept : x(X), y(Y) {} //!< setup

        //!assign
        inline  v2d & operator=( const v2d &v ) noexcept
        {
            x = v.x;
            y = v.y;
            return *this;
        }

        //______________________________________________________________________
        //
        // pseudo array
        //______________________________________________________________________
        //! size
        inline size_t size() const noexcept { return 2; }

        //! access
        inline type & operator[](size_t indx) noexcept {
            assert(indx>=1);assert(indx<=2);
            return *( &x + --indx );
        }

        //! access, const
        inline const_type & operator[](size_t indx) const noexcept {
            assert(indx>=1);assert(indx<=2);
            return *( &x + --indx );
        }

        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const v2d &v)
        {
            os << '[' << v.x << ';' << v.y << ']';
            return os;
        }

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________

        //! add
        inline friend v2d operator+ ( const v2d &lhs, const v2d &rhs ) noexcept {
            return v2d(lhs.x+rhs.x,lhs.y+rhs.y);
        }

        //! add
        inline v2d & operator+=(const v2d &rhs) noexcept {
            x += rhs.x; y += rhs.y; return *this;
        }

        //! unary +
        inline v2d operator+() const noexcept {
            return *this;
        }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! sub
        inline friend v2d operator- ( const v2d &lhs, const v2d &rhs ) noexcept {
            return v2d(lhs.x-rhs.x,lhs.y-rhs.y);
        }

        //! sub
        inline v2d & operator-=(const v2d &rhs) noexcept {
            x -= rhs.x; y -= rhs.y; return *this;
        }

        //! unary -
        inline v2d operator-() const noexcept {
            return v2d(-x,-y);
        }
        
        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________

        //! by scalar
        inline friend v2d operator * (const T factor, const v2d &rhs) noexcept
        {
            return v2d(factor*rhs.x, factor*rhs.y);
        }

        //! by scalar
        inline friend v2d operator * ( const v2d &lhs, const T factor) noexcept
        {
            return v2d(factor*lhs.x, factor*lhs.y);
        }

        //! in place
        inline v2d & operator*=(const T factor) noexcept
        {
            x*=factor;
            y*=factor;
            return *this;
        }

        //! scalar product
        inline friend T operator*( const v2d &lhs, const v2d &rhs) noexcept
        {
            return lhs.x*rhs.x + lhs.y*rhs.y;
        }

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        //! by scalar
        inline friend v2d operator / ( const v2d &lhs, const T factor) noexcept
        {
            return v2d(lhs.x/factor,lhs.y/factor);
        }

        //! in place
        inline v2d & operator/=(const T factor) noexcept
        {
            x/=factor;
            y/=factor;
            return *this;
        }



        //______________________________________________________________________
        //
        // norms
        //______________________________________________________________________
        
        //! norm2
        inline mutable_type norm2() const noexcept
        {
            return squared(x) + squared(y);
        }

        //! norm
        inline mutable_type norm() const noexcept
        {
            return math::hypothenuse(x,y);
        }

        //! norm1
        inline mutable_type norm1() const noexcept
        {
            return absolute(x) + absolute(y);
        }

        //! angle in -pi:pi for float,double, long double
        mutable_type angle() const noexcept;

        //______________________________________________________________________
        //
        // comparisons
        //______________________________________________________________________

        //! test equality
        inline friend bool operator==(const v2d &lhs, const v2d &rhs) noexcept
        {
            return lhs.x==rhs.x && lhs.y==rhs.y;
        }

        //! test different
        inline friend bool operator!=(const v2d &lhs, const v2d &rhs) noexcept
        {
            return lhs.x!=rhs.x || lhs.y!=rhs.y;
        }
        
        //! lexicographic comparions
        static inline int compare(const v2d &lhs, const v2d &rhs) noexcept
        {
            const T *l = &lhs.x; const T *r = &rhs.x;
            return comparison::lexicographic(l,r,2);
        }
        
        
    };
}

#endif
