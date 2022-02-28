
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
            template <typename T>
            class rgba
            {
            public:
                YACK_DECL_ARGS(T,type);
                typedef opaque<mutable_type> opaque_type;

                type r;
                type g;
                type b;
                type a;

                inline  rgba() : r(0), g(0), b(0), a(opaque_type::value) {}
                inline  rgba(param_type R, param_type G, param_type B) : r(R), g(G), b(B), a(opaque_type::value) {}
                inline  rgba(param_type R, param_type G, param_type B, param_type A) : r(R), g(G), b(B), a(A) {}
                inline ~rgba() throw() {}
                inline  rgba(const rgba &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
                inline  rgba & operator=(const rgba &other)
                {
                    r = other.r;
                    g = other.g;
                    b = other.b;
                    a = other.a;
                    return *this;
                }

                inline rgba(const rgb<T> &c) : r(c.r), g(c.g), b(c.b), a(opaque_type::value) {}


                inline friend bool operator==(const rgba &lhs, const rgba &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b) && (lhs.a==rhs.a);
                }

            };

            template <typename T>
            inline rgb<T>::rgb(const rgba<T> &c) : r(c.r), g(c.g), b(c.b) {}

        }

        typedef nexus::rgba<uint8_t> rgba;

    }
}

#endif

