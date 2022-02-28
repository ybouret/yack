//! \file

#ifndef YACK_COLOR_RGB_INCLUDED
#define YACK_COLOR_RGB_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    namespace color
    {
        namespace nexus
        {

            template <typename T> class rgba;

            template <typename T>
            class rgb
            {
            public:
                YACK_DECL_ARGS(T,type);

                type r;
                type g;
                type b;

                inline  rgb() : r(0), g(0), b(0) {}
                inline  rgb(param_type R, param_type G, param_type B) : r(R), g(G), b(B) {}
                inline ~rgb() throw() {}
                inline  rgb(const rgb &c) : r(c.r), g(c.g), b(c.b) {}
                inline  rgb & operator=(const rgb &other)
                {
                    r = other.r;
                    g = other.g;
                    b = other.b;
                    return *this;
                }

                inline rgb(const rgba<T> &);

                inline friend bool operator==(const rgb &lhs, const rgb &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

            };
        }

        typedef nexus::rgb<uint8_t> rgb;
        
    }
}

#endif

