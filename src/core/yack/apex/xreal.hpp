//! \file

#ifndef YACK_APEX_XREAL_INCLUDED
#define YACK_APEX_XREAL_INCLUDED 1

#include "yack/signs.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        //! value = m * 10^p
        template <typename T>
        class xreal
        {
        public:
            static const T   maximum;
            static const T   minimum;
            static T         ten_to(unit_t q) noexcept;
            const T          m;
            const unit_t     p;

            xreal(const T     r);
            xreal(const xreal &) noexcept;
            ~xreal() noexcept;
            xreal & operator=(const xreal &) noexcept;

            friend inline std::ostream & operator<<(std::ostream &os,const xreal<T> &x)
            {
                os << x.m;
                switch( __sign::of(x.p) )
                {
                    case __zero__: break;
                    case positive:
                    case negative: os << 'E' << x.p; break;
                }
                return os;
            }

            xreal operator+() const noexcept;
            xreal operator-() const noexcept;

            xreal &      operator*=(const xreal rhs);
            friend xreal operator*(xreal lhs, const xreal rhs)
            {
                return lhs *= rhs;
            }


        private:
            xreal(const T m_, const unit_t p_) noexcept;
        };
    }
}

#endif

