//! \file

#ifndef YACK_TYPE_V3D_INCLUDED
#define YACK_TYPE_V3D_INCLUDED 1

#include "yack/type/v2d.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! 3D vertex
    //
    //__________________________________________________________________________
    template <typename T>
    class v3d
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
        type z; //!< z

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  v3d() throw() : x(0), y(0), z(0) {}                     //!< setup
        inline ~v3d() throw() {}                                        //!< cleanup
        inline  v3d(const v3d &v) throw() : x(v.x), y(v.y), z(v.z) {}   //!< copy
        //!
        //! setup
        inline  v3d(const_type X,
                    const_type Y,
                    const_type Z) throw() : x(X), y(Y), z(Z) {}

        //! assign
        inline  v3d & operator=( const v3d &v ) throw()
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        //______________________________________________________________________
        //
        // pseudo array
        //______________________________________________________________________
        //! size
        inline size_t size() const throw() { return 3; }

        //! access
        inline type & operator[](size_t indx) throw() {
            assert(indx>=1);assert(indx<=3);
            return *( &x + --indx );
        }

        //! access, const
        inline const_type & operator[](size_t indx) const throw() {
            assert(indx>=1);assert(indx<=3);
            return *( &x + --indx );
        }

        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const v3d &v)
        {
            os << '[' << v.x << ';' << v.y << ';' << v.z << ']';
            return os;
        }

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________

        //! add
        inline friend v3d operator+ ( const v3d &lhs, const v3d &rhs ) throw() {
            return v3d(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z);
        }

        //! add
        inline v3d & operator+=(const v3d &rhs) throw() {
            x += rhs.x; y += rhs.y; z += rhs.z; return *this;
        }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! sub
        inline friend v3d operator- ( const v3d &lhs, const v3d &rhs ) throw() {
            return v3d(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z);
        }

        //! sub
        inline v3d & operator-=(const v3d &rhs) throw() {
            x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
        }

        //______________________________________________________________________
        //
        // norms
        //______________________________________________________________________
        inline mutable_type norm2() const throw()
        {
            return squared(x) + squared(y) + squared(z);
        }

    };
}

#endif
