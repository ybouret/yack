
//! \file

#ifndef YACK_GFX_RGB_INCLUDED
#define YACK_GFX_RGB_INCLUDED 1

#include "yack/type/args.hpp"


namespace yack
{
    namespace graphic
    {

        template <typename T>
        struct opaque
        {
            static const T value;
        };

        namespace nexus
        {
            template <typename T>
            class rgb
            {
            public:
                YACK_DECL_ARGS(T,type);

                type r;
                type g;
                type b;

                inline  rgb() : r(0), g(0), b(0) {}
                inline ~rgb() throw() {}
                inline  rgb(const rgb &c) : r(c.r), g(c.g), b(c.b) {}
                inline  rgb & operator=(const rgb &other)
                {
                    r = other.r;
                    g = other.g;
                    b = other.b;
                    return *this;
                }

                inline friend bool operator==(const rgb &lhs, const rgbs &rhs) throw()
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

            };
        }

        typedef nexus::rgb<uint8_t> rgb;

    }

}

#endif
