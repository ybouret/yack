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

            template <typename T> class rgba; //!< forward declaration

            //__________________________________________________________________
            //
            //
            //! base class for rgb
            //
            //__________________________________________________________________
            template <typename T>
            class rgb
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                YACK_DECL_ARGS(T,type); //!< aliases

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline  rgb() : r(0), g(0), b(0) {}

                //! setup
                inline  rgb(param_type R, param_type G, param_type B) : r(R), g(G), b(B) {}

                //! cleanup
                inline ~rgb() noexcept {}

                //! copy
                inline  rgb(const rgb &c) : r(c.r), g(c.g), b(c.b) {}

                //! assign
                inline  rgb & operator=(const rgb &other)
                {
                    r = other.r;
                    g = other.g;
                    b = other.b;
                    return *this;
                }

                //! assign from rgba
                inline rgb(const rgba<T> &);

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________

                //! field wise comparison
                inline friend bool operator==(const rgb &lhs, const rgb &rhs) noexcept
                {
                    return (lhs.r==rhs.r) && (lhs.g==rhs.g) && (lhs.b==rhs.b);
                }

                //! helper
                inline size_t size() const noexcept { return 3; }

                //! access helper
                inline type &operator[](const size_t indx) noexcept
                {
                    assert(indx>=1); assert(indx<=3);
                    return *((&r-1)+indx);
                }

                //! const access helper
                inline const_type &operator[](const size_t indx) const noexcept
                {
                    assert(indx>=1); assert(indx<=3);
                    return *((&r-1)+indx);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                type r; //!< red
                type g; //!< green
                type b; //!< blue


            };
        }

        typedef nexus::rgb<uint8_t> rgb; //!< default rgb
        
    }
}

#endif

