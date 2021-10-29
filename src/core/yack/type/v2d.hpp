
//! \file

#ifndef YACK_TYPE_V2D_INCLUDED
#define YACK_TYPE_V2D_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/utils.hpp"
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
        inline  v2d() throw() : x(0), y(0) {}                           //!< setup
        inline ~v2d() throw() {}                                        //!< cleanup
        inline  v2d(const v2d &v) throw() : x(v.x), y(v.y) {}           //!< copy
        inline  v2d(const_type X, const_type Y) throw() : x(X), y(Y) {} //!< setup

        //!assign
        inline  v2d & operator=( const v2d &v ) throw()
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
        inline size_t size() const throw() { return 2; }

        //! access
        inline type & operator[](size_t indx) throw() {
            assert(indx>=1);assert(indx<=2);
            return *( &x + --indx );
        }

        //! access, const
        inline const_type & operator[](size_t indx) const throw() {
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
        inline friend v2d operator+ ( const v2d &lhs, const v2d &rhs ) throw() {
            return v2d(lhs.x+rhs.x,lhs.y+rhs.y);
        }

        //! add
        inline v2d & operator+=(const v2d &rhs) throw() {
            x += rhs.x; y += rhs.y; return *this;
        }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! sub
        inline friend v2d operator- ( const v2d &lhs, const v2d &rhs ) throw() {
            return v2d(lhs.x-rhs.x,lhs.y-rhs.y);
        }

        //! sub
        inline v2d & operator-=(const v2d &rhs) throw() {
            x -= rhs.x; y -= rhs.y; return *this;
        }

        //______________________________________________________________________
        //
        // norms
        //______________________________________________________________________
        inline mutable_type norm2() const throw()
        {
            return squared(x) + squared(y);
        }
        
    };
}

#endif
