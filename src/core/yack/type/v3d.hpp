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
        inline  v3d() noexcept : x(0), y(0), z(0) {}                     //!< setup
        inline ~v3d() noexcept {}                                        //!< cleanup
        inline  v3d(const v3d &v) noexcept : x(v.x), y(v.y), z(v.z) {}   //!< copy

        //! setup
        inline  v3d(const_type X,
                    const_type Y,
                    const_type Z) noexcept : x(X), y(Y), z(Z) {}

        //! assign
        inline  v3d & operator=( const v3d &v ) noexcept
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
        inline size_t size() const noexcept { return 3; }

        //! access
        inline type & operator[](size_t indx) noexcept {
            assert(indx>=1);assert(indx<=3);
            return *( &x + --indx );
        }

        //! access, const
        inline const_type & operator[](size_t indx) const noexcept {
            assert(indx>=1);assert(indx<=3);
            return *( &x + --indx );
        }

        //! return v2d(x,y)/
        v2d<T> xy() const noexcept { return v2d<T>(x,y); }

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
        inline friend v3d operator+ ( const v3d &lhs, const v3d &rhs ) noexcept {
            return v3d(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z);
        }

        //! add
        inline v3d & operator+=(const v3d &rhs) noexcept {
            x += rhs.x; y += rhs.y; z += rhs.z; return *this;
        }

        //! unary plus
        inline v3d operator+() const noexcept { return *this; }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! sub
        inline friend v3d operator- ( const v3d &lhs, const v3d &rhs ) noexcept {
            return v3d(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z);
        }

        //! sub
        inline v3d & operator-=(const v3d &rhs) noexcept {
            x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
        }

        //! unary minus
        inline v3d operator-() const noexcept { return v3d(-x,-y,-z); }

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________

        //! by scalar
        inline friend v3d operator * (const T factor, const v3d &rhs) noexcept
        {
            return v3d(factor*rhs.x, factor*rhs.y, factor*rhs.z);
        }

        //! by scalar
        inline friend v3d operator * ( const v3d &lhs, const T factor) noexcept
        {
            return v3d(factor*lhs.x, factor*lhs.y, factor*lhs.z);
        }

        //! by scalar
        inline v3d & operator*=(const T factor) noexcept
        {
            x*=factor;
            y*=factor;
            z*=factor;
            return *this;
        }

        //! scalar product
        inline friend T operator*(const v3d &lhs, const v3d &rhs) noexcept
        {
            return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
        }

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        //! by scalar
        inline friend v3d operator / ( const v3d &lhs, const T factor) noexcept
        {
            return v3d(lhs.x/factor,lhs.y/factor,lhs.z/factor);
        }

        //! in place
        inline v3d & operator/=(const T factor) noexcept
        {
            x/=factor;
            y/=factor;
            z/=factor;
            return *this;
        }

        //______________________________________________________________________
        //
        // norms
        //______________________________________________________________________
      
        //! norm2
        inline mutable_type norm2() const noexcept
        {
            return squared(x) + squared(y) + squared(z);
        }

        //! norm1
        inline mutable_type norm1() const noexcept
        {
            return absolute(x) + absolute(y) + absolute(z);
        }

        //! test equality
        inline friend bool operator==(const v3d &lhs, const v3d &rhs) noexcept
        {
            return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
        }

        //! test different
        inline friend bool operator!=(const v3d &lhs, const v3d &rhs) noexcept
        {
            return lhs.x!=rhs.x || lhs.y!=rhs.y || lhs.z != rhs.z;
        }
        
        //! lexicographic comparison
        static inline int compare(const v3d &lhs, const v3d &rhs) noexcept
        {
            const T *l = &lhs.x; const T *r = &rhs.x;
            return comparison::lexicographic(l,r,3);
        }
    };
}

#endif
