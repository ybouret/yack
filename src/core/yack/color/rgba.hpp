
//! \file

#ifndef YACK_COLOR_RGBA_INCLUDED
#define YACK_COLOR_RGBA_INCLUDED 1

#include "yack/color/rgb.hpp"
#include "yack/color/opaque.hpp"

namespace yack
{
    namespace color
    {
        namespace nexus
        {
            //__________________________________________________________________
            //
            //
            //! base class for rgba
            //
            //__________________________________________________________________
            template <typename T>
            class rgba
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                YACK_DECL_ARGS(T,type);                   //!< aliases
                typedef opaque<mutable_type> opaque_type; //!< alias



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline  rgba() : r(0), g(0), b(0), a(opaque_type::value) {}

                //! setup
                inline  rgba(param_type R, param_type G, param_type B) : r(R), g(G), b(B), a(opaque_type::value) {}

                //! setup
                inline  rgba(param_type R, param_type G, param_type B, param_type A) : r(R), g(G), b(B), a(A) {}

                //! cleanup
                inline ~rgba() noexcept {}

                //! copy
                inline  rgba(const rgba &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

                //! assign
                inline  rgba & operator=(const rgba &other)
                {
                    r = other.r;
                    g = other.g;
                    b = other.b;
                    a = other.a;
                    return *this;
                }

                //! setup from rgb
                inline rgba(const rgb<T> &c) : r(c.r), g(c.g), b(c.b), a(opaque_type::value) {}


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! field wise comparator
                inline friend bool operator==(const rgba &lhs, const rgba &rhs) noexcept
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b) && (lhs.a==rhs.a);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                type r; //!< red
                type g; //!< green
                type b; //!< blue
                type a; //!< alpha

            };

            //! specific
            template <typename T>
            inline rgb<T>::rgb(const rgba<T> &c) : r(c.r), g(c.g), b(c.b) {}

        }

        //! default rgba
        typedef nexus::rgba<uint8_t> rgba;

    }
}

#endif

