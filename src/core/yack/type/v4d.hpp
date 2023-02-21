
//! \file

#ifndef YACK_TYPE_V4D_INCLUDED
#define YACK_TYPE_V4D_INCLUDED 1

#include "yack/type/v3d.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! 4D vertex
    //
    //__________________________________________________________________________
    template <typename T>
    class v4d
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
        type w; //!< w

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  v4d() noexcept : x(0), y(0), z(0), w(0) {}                       //!< setup
        inline ~v4d() noexcept {}                                                //!< cleanup
        inline  v4d(const v4d &v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}   //!< copy
        //!
        //! setup
        inline  v4d(const_type X,
                    const_type Y,
                    const_type Z,
                    const_type W) noexcept : x(X), y(Y), z(Z), w(W) {}

        //! assign
        inline  v4d & operator=( const v4d &v ) noexcept
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        //______________________________________________________________________
        //
        // pseudo array
        //______________________________________________________________________
        //! size
        inline size_t size() const noexcept { return 4; }

        //! access
        inline type & operator[](size_t indx) noexcept {
            assert(indx>=1);assert(indx<=4);
            return *( &x + --indx );
        }

        //! access, const
        inline const_type & operator[](size_t indx) const noexcept {
            assert(indx>=1);assert(indx<=4);
            return *( &x + --indx );
        }

        //! get xyz
        inline const v3d<T> xyz() const noexcept { return v3d<T>(x,y,z); }

        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const v4d &v)
        {
            os << '[' << v.x << ';' << v.y << ';' << v.z << ';' << v.w << ']';
            return os;
        }

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________

        //! add
        inline friend v4d operator+ ( const v4d &lhs, const v4d &rhs ) noexcept {
            return v4d(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z,lhs.w+rhs.w);
        }

        //! add
        inline v4d & operator+=(const v4d &rhs) noexcept {
            x += rhs.x; y += rhs.y; z += rhs.z; w+=rhs.w; return *this;
        }

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! sub
        inline friend v4d operator- ( const v4d &lhs, const v4d &rhs ) noexcept {
            return v4d(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z,lhs.w-rhs.w);
        }

        //! sub
        inline v4d & operator-=(const v4d &rhs) noexcept {
            x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
        }

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________

        //! by scalar
        inline friend v4d operator * (const T factor, const v4d &rhs) noexcept
        {
            return v4d(factor*rhs.x, factor*rhs.y, factor*rhs.z, factor*rhs.w);
        }

        //! by scalar
        inline friend v4d operator * ( const v4d &lhs, const T factor) noexcept
        {
            return v4d(factor*lhs.x, factor*lhs.y, factor*lhs.z, factor*lhs.w);
        }

        //! by scalar
        inline v4d & operator*=(const T factor) noexcept
        {
            x*=factor;
            y*=factor;
            z*=factor;
            w*=factor;
            return *this;
        }


        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        //! by scalar
        inline friend v4d operator / ( const v4d &lhs, const T factor) noexcept
        {
            return v4d(lhs.x/factor,lhs.y/factor,lhs.z/factor,lhs.w/factor);
        }

        //! in place
        inline v4d & operator/=(const T factor) noexcept
        {
            x/=factor;
            y/=factor;
            z/=factor;
            w/=factor;
            return *this;
        }

        //______________________________________________________________________
        //
        // norms
        //______________________________________________________________________

        //! norm2
        inline mutable_type norm2() const noexcept
        {
            return squared(x) + squared(y) + squared(z) + squared(w);
        }

        //! norm1
        inline mutable_type norm1() const noexcept
        {
            return absolute(x) + absolute(y) + absolute(z) + absolute(w);
        }


        //! test equality
        inline friend bool operator==(const v4d &lhs, const v4d &rhs) noexcept
        {
            return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z && lhs.w==rhs.w;
        }

        //! test different
        inline friend bool operator!=(const v4d &lhs, const v4d &rhs) noexcept
        {
            return lhs.x!=rhs.x || lhs.y!=rhs.y || lhs.z != rhs.z || lhs.w != lhs.w;
        }

        //! lexicographic comparison
        static inline int compare(const v4d &lhs, const v4d &rhs) noexcept
        {
            const T *l = &lhs.x; const T *r = &rhs.x;
            return comparison::lexicographic(l,r,4);
        }
    };
}

#endif
